#include "timetable_screen_builder.h"
#include <iostream>
#include <sstream>
#include <assert.h>

TimetableScreenBuilder::TimetableScreenBuilder(const Options &options):
  canvas(options.getWidth(), options.getHeight()),
  line_height(options.getHeight() / options.getLines()),
  title_width(options.getWidth() / 5),
  study_width(options.getWidth() / 14),
  day_width((options.getWidth() - title_width - study_width) / 7),
  testing(options.isTesting()),
  color_clear(options.getClearColor()),
  title_face(options.getTitleFace()),
  header_back(options.getHeaderBack()),
  header_face(options.getHeaderFace()),
  line_odd(options.getLineOdd()),
  line_even(options.getLineEven()),
  line_face(options.getLineFace()),
  counter_face(options.getCounterFace()),
  font_file(options.getFontFile()),
  title_name(options.getTitleName()),
  title_study(options.getTitleStudy()),
  title_no_time(options.getTitleNoTime()),
  title_monday(options.getTitleMonday()),
  title_tuesday(options.getTitileTuesday()),
  title_wednesday(options.getTitleWednesday()),
  title_thursday(options.getTitleThursday()),
  title_fryday(options.getTitleFryday()),
  title_saturday(options.getTitleSaturday()),
  title_sunday(options.getTitleSunday()),
  title_testing(options.getTitleTesting())
{
}

bool TimetableScreenBuilder::init(void) {
  int fsize = line_height;
  return font_title.init(font_file, fsize, 72)
    && font_caption.init(font_file, fsize * 3 / 5, 72)
    && font_text.init(font_file, fsize * 8 / 19, 72);
}

TimetableScreenBuilder::~TimetableScreenBuilder() {}

image::Canvas& TimetableScreenBuilder::getCanvas(void) { return canvas; }
const unsigned char* TimetableScreenBuilder::getPixels(void) const { return canvas.getPixels(); }
unsigned char* TimetableScreenBuilder::getPixels(void) { return canvas.getPixels(); }

int TimetableScreenBuilder::getWidth(void) const { return canvas.getWidth(); }
int TimetableScreenBuilder::getHeight(void) const { return canvas.getHeight(); }
int TimetableScreenBuilder::getSize(void) const { return canvas.getSize(); }

void TimetableScreenBuilder::build(const Profiles &ps, int page, int pageCount) {
  int y = ps.size() * 3 + 1;

  for (const timetable::Profile &profile : ps)
    y += profile.getDoctors().size();

  // Center timetables vertically
  y = (canvas.getHeight() - y * line_height) / (ps.size() + 1);

  // Vertical distance between timetable profiles
  int dy = y;

  canvas.clear(color_clear);

  for (const timetable::Profile &profile : ps) {
    image::Rect rect = { 0, y, canvas.getWidth() - 1, y + line_height * 2};

    canvas.textOut(profile.getTitle(), font_title, title_face, rect, true);

    const std::vector<timetable::Doctor> &ds = profile.getDoctors();
    int len = ds.size();

    // Make sure we'll make no overflow
    assert(y + line_height * (len + 3) < canvas.getHeight());

    // Box
    rect.y0 = y + line_height * 2;
    rect.y1 = rect.y0 + line_height * (len + 1);
    canvas.fillBox(line_even, rect);

    // Header
    rect.y0 = y + line_height * 2 + 1;
    rect.y1 = y + line_height * 3 - 1;
    canvas.fillBox(header_back, rect);

    // Line stripes
    rect.y0 = y + line_height * 3 + 1;
    rect.y1 = y + line_height * 4 - 1;
    for (int i = 0; i < len; i += 2) {
      canvas.fillBox(line_odd, rect);
      rect.y0 += line_height * 2;
      rect.y1 += line_height * 2;
    }

    // Columns
    rect.y0 = y + line_height * 2;
    rect.y1 = rect.y0 + line_height * (len + 1);

    canvas.vertical(line_even, title_width, rect.y0, rect.y1);
    canvas.vertical(line_even, title_width + study_width, rect.y0, rect.y1);

    for (int i = 1; i < 7; ++i) {
      canvas.vertical(line_even,
        title_width + study_width + i * day_width,
        rect.y0,
        rect.y1);
    }

    // Table titles
    //  Doctor
    rect.y0 = y + line_height * 2 + 1;
    rect.y1 = y + line_height * 3 - 1;
    rect.x0 = 3;
    rect.x1 = title_width - 1;
    canvas.textOut(title_name, font_caption, header_face, rect, true);

    //   Study
    rect.x0 = title_width + 1;
    rect.x1 = title_width + study_width - 1;
    canvas.textOut(title_study, font_caption, header_face, rect, true);

    rect.x0 = title_width + study_width;
    rect.x1 = rect.x0 + day_width;
    canvas.textOut(title_monday, font_caption, header_face, rect, true);

    rect.x0 += day_width;
    rect.x1 += day_width;
    canvas.textOut(title_tuesday, font_caption, header_face, rect, true);

    rect.x0 += day_width;
    rect.x1 += day_width;
    canvas.textOut(title_wednesday, font_caption, header_face, rect, true);

    rect.x0 += day_width;
    rect.x1 += day_width;
    canvas.textOut(title_thursday, font_caption, header_face, rect, true);

    rect.x0 += day_width;
    rect.x1 += day_width;
    canvas.textOut(title_fryday, font_caption, header_face, rect, true);

    rect.x0 += day_width;
    rect.x1 += day_width;
    canvas.textOut(title_saturday, font_caption, header_face, rect, true);

    rect.x0 += day_width;
    rect.x1 += day_width;
    canvas.textOut(title_sunday, font_caption, header_face, rect, true);

    // Doctors
    rect.y0 = y + line_height * 3 + 1;
    rect.y1 = y + line_height * 4 - 1;
    for (const timetable::Doctor &doc : ds) {
      rect.x0 = line_height / 4;
      rect.x1 = title_width - 1;
      canvas.textOut(doc.getName(), font_text, line_face, rect, false);

      rect.x0 = title_width;
      rect.x1 = title_width + study_width;
      canvas.textOut(doc.getStudy(), font_text, line_face, rect, true);

      rect.x0 = title_width + study_width;
      rect.x1 = rect.x0 + day_width;
      for (int i = 0; i < 7; ++i) {
        const timetable::ReceiptTime &r = doc[(timetable::Weekdays)i];
        if (r.blocked) {
          canvas.textOut(title_no_time, font_text, line_face, rect, true);
        } else {
          canvas.textOut(r.title, font_text, line_face, rect, true);
        }
        rect.x0 += day_width;
        rect.x1 += day_width;
      }
      rect.y0 += line_height;
      rect.y1 += line_height;
    }

    y += line_height * (len + 3) + dy;
  }
  
  std::ostringstream pager;
  if (testing) {
    pager << "***    " << title_testing << "    ***    " << page << " / " << pageCount << "    ***    " << title_testing << "    ***";
  } else {
    pager << page << " / " << pageCount;
  }
  
  image::Rect rect = { 0,
    canvas.getHeight() - line_height,
    canvas.getWidth() - 1,
    canvas.getHeight() - 1
  };
  
  canvas.textOut(pager.str(), font_caption, counter_face, rect, true);
}
