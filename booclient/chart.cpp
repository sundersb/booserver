#include "chart.h"

#include <wx/dcmemory.h>

#include <string>
#include <algorithm>
#include <functional>

const int WEEK_LENGTH = 7;
const int LINE_WIDTH = 2;

const int CELL_WIDTH = 80;
const int CHART_WIDTH = CELL_WIDTH * WEEK_LENGTH;
const int CHART_HEIGHT = 70;

const int FONT_SIZE_DATE = CELL_WIDTH * 5 / 6;
const int FONT_SIZE_MONTH = CELL_WIDTH / 3;
const int FONT_SIZE_TIME = CELL_WIDTH / 7;
const int MONTH_Y = CHART_HEIGHT - FONT_SIZE_TIME * 4;
const int TITLE_Y = CHART_HEIGHT - FONT_SIZE_TIME - 2;

const wxColour COLOR_DATES(0x99, 0xcc, 0xff);
const wxColour COLOR_DATES_OFF(0xff, 0x99, 0x66);
const wxColour COLOR_BGR(0xff, 0xff, 0xcc);
const wxColour COLOR_LINE(0xcc, 0xcc, 0xcc);
const wxColour COLOR_MONTH(0xaa, 0x55, 0x55);

const wxBrush BRUSH_BACKGROUND(COLOR_BGR);
const wxPen PEN_LINE(COLOR_LINE, LINE_WIDTH, wxPENSTYLE_SOLID);

// Title for a day for which no rule found ("none")
const wxString NOT_FOUND ("\u043d\u0435\u0442", wxConvUTF8);

Chart::Chart():
  week(CHART_WIDTH, CHART_HEIGHT, 24),
  bitmap(CHART_WIDTH, CHART_HEIGHT, 24),
  monday()
{
  // Set on the current week's beginning
  monday.setNow();
  monday.discardTime();
  int wd = monday.getWeekDay();
  monday.incDay(-wd);

  // Draw week's grid
  update();
}

Chart::~Chart() {}

void Chart::update(void) {
  wxMemoryDC dc(week);

  dc.SetBackground(BRUSH_BACKGROUND);
  dc.Clear();

  dc.SetPen(PEN_LINE);

  wxFont dfont(wxSize(0, FONT_SIZE_DATE), wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
  wxFont mfont(wxSize(0, FONT_SIZE_MONTH), wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
  wxFont font(wxSize(0, FONT_SIZE_TIME), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

  // Draw day cells
  DateTime date (monday);
  for (int i = 0; i < WEEK_LENGTH; ++i) {
    tm time;
    wxCoord x, y;

    // Split date on parts
    date.asTime(&time);

    // Day number
    wxString title(std::to_string(time.tm_mday).c_str(), wxConvUTF8);
    dc.GetTextExtent(title, &x, &y, nullptr, nullptr, &dfont);

    x = i * CELL_WIDTH + (CELL_WIDTH - x) / 2;
    y = (CHART_HEIGHT - y) / 2;

    dc.SetFont(dfont);
    if (i < 5)
      dc.SetTextForeground(COLOR_DATES);
    else
      dc.SetTextForeground(COLOR_DATES_OFF);

    dc.DrawText(title, x, y);

    // Day separator line
    x = i * CELL_WIDTH;
    dc.DrawLine(x, 0, x, CHART_HEIGHT - 1);

    // Month name
    title = wxDateTime::GetMonthName(wxDateTime::Month(time.tm_mon), wxDateTime::Name_Abbr);
    dc.GetTextExtent(title, &x, nullptr, nullptr, nullptr, &mfont);
    x = (i + 1) * CELL_WIDTH - x;
    dc.SetFont(mfont);
    dc.SetTextForeground(COLOR_MONTH);
    dc.DrawText(title, x, MONTH_Y);

    // Weekday name
    title = wxDateTime::GetWeekDayName(wxDateTime::WeekDay(i > 5 ? 0 : i + 1), wxDateTime::Name_Abbr);
    dc.GetTextExtent(title, &x, nullptr, nullptr, nullptr, &font);
    x = i * CELL_WIDTH + (CELL_WIDTH - x) / 2;
    dc.SetFont(font);
    dc.SetTextForeground(*wxBLACK);
    dc.DrawText(title, x, 3);

    date.incDay(1);
  }

  // Copy grid on the working bitmap
  dc.SelectObject(bitmap);
  dc.DrawBitmap(week, 0, 0, false);
}

void Chart::goBack(void) {
  monday.incDay(-WEEK_LENGTH);
  update();
}

void Chart::goForth(void) {
  monday.incDay(WEEK_LENGTH);
  update();
}

const wxBitmap& Chart::get(void) const {
  return bitmap;
}

const wxBitmap& Chart::get(std::vector<Rule*> &rules) {
  wxMemoryDC dc(bitmap);
  dc.DrawBitmap(week, 0, 0, false);

  wxFont font(wxSize(0, FONT_SIZE_TIME), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  dc.SetFont(font);
  dc.SetTextForeground(*wxBLACK);

  // Draw days
  DateTime date(monday);
  for (int i = 0; i < WEEK_LENGTH; ++i) {
    // Find first working rule for the date
    std::vector<Rule*>::const_iterator day = std::find_if(rules.begin(),
      rules.end(),
      std::bind(&Rule::fits, std::placeholders::_1, date));

    wxString title = NOT_FOUND;
    if (day != rules.end()) {
      // Rule found
      title = wxString((*day)->getTitle().c_str(), wxConvUTF8);
    }

    wxCoord x;
    dc.GetTextExtent(title, &x, nullptr, nullptr, nullptr, &font);

    x = i * CELL_WIDTH + (CELL_WIDTH - x) / 2;

    dc.DrawText(title, x, TITLE_Y);
    date.incDay(1);
  }

  return bitmap;
}
