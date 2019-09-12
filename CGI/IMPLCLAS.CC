#define IMPLEMENT_container
#define IMPLEMENT_array_of
#define IMPLEMENT_iterator
#include "uses_wb.hpp"

ostream& operator << (ostream&,const container<const char*>&);

static error_handling dummy;

