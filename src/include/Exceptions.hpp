#pragma once

#include <exception>
#include <stdexcept>
using namespace std;

namespace potato {

class walked_off_edge : public std::logic_error {
    using std::logic_error::logic_error;
};
class not_implemented : public std::logic_error {
    using std::logic_error::logic_error;
};
class syntax_error : public std::logic_error {
    using std::logic_error::logic_error;
};


};
