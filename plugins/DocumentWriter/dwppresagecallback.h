#ifndef DWPPRESAGECALLBACK_H
#define DWPPRESAGECALLBACK_H

#include <presage.h>
#include <iostream>

#include "documentwriterplugin.h"

class DWPPresageCallback : public PresageCallback
{
public:
    DWPPresageCallback(DWPTextEdit *_past_context) : past_context(_past_context) { }

    std::string get_past_stream() const { return past_context->toPlainText().toStdString(); }
    std::string get_future_stream() const { return empty; }

private:
    DWPTextEdit *past_context;
    const std::string empty;
};

#endif // DWPPRESAGECALLBACK_H
