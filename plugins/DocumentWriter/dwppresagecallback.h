#ifndef DWPPRESAGECALLBACK_H
#define DWPPRESAGECALLBACK_H

#include <presage.h>
#include <iostream>

#include "documentwriterplugin.h"

class DWPPresageCallback : public PresageCallback
{
public:
    DWPPresageCallback(const std::string& _past_context) : past_context(_past_context) { }

    std::string get_past_stream() const {
      std::cout << "past_context: " << past_context << std::endl << std::flush;
      return past_context;
    }
    std::string get_future_stream() const { return empty; }

private:
    const std::string& past_context;
    const std::string empty;

};

#endif // DWPPRESAGECALLBACK_H
