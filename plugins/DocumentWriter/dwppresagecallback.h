#ifndef DWPPRESAGECALLBACK_H
#define DWPPRESAGECALLBACK_H

#ifdef SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED

#include <presage.h>
#include <iostream>

#include "documentwriterplugin.h"

class DWPPresageCallback : public PresageCallback
{
public:
    DWPPresageCallback(const std::string& _past_context) : past_context(_past_context) { }

    std::string get_past_stream() const {
      return past_context;
    }
    std::string get_future_stream() const { return empty; }

private:
    const std::string& past_context;
    const std::string empty;

};

#endif

#endif // DWPPRESAGECALLBACK_H
