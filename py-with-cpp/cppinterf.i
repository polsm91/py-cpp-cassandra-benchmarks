%module cppinterf
%{
 #include<cassandra.h>
 #include "cppinterf.h"
 extern void init_globals();
%}
#include "cppinterf.h"
%init %{
    init_globals();
%}
