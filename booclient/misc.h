#pragma once

#include <wx/wx.h>
#include <wx/ctrlsub.h>
#include <wx/window.h>

/**
 * @brief             Clear control with items, freeing attached models
 * @param box         Itemed control
 */
template <class T>
void clearItems(wxItemContainer *box) {
  unsigned j = box->GetCount();
  for (unsigned i = 0; i < j; ++i) {
    T *p = static_cast<T*> (box->GetClientData(i));
    box->SetClientData(i, nullptr);
    delete p;
  }
  box->Clear();
}

/**
 * @brief             Find index of an item by attached data on a control with item
 * @param box         Control to search
 * @param data        Data to match against ClientData of the control's items
 * @return            Index of the item pointing to the data or wxNOT_FOUND if
 *                    no such item found
 */
long findItem(wxItemContainer *box, void *data) {
  long j = box->GetCount();
  for (long i = 0; i < j; ++i) {
    if (data == box->GetClientData(i)) return i;
  }
  return wxNOT_FOUND;
}

class Freezer {
  wxWindow *window;

  Freezer() = delete;
  Freezer(Freezer&) = delete;
  void operator= (Freezer&) = delete;

public:
  Freezer(wxWindow *value): window(value) {
    if(window) window->Freeze();
  }
  ~Freezer() {
    if (window) window->Thaw();
  }
};
