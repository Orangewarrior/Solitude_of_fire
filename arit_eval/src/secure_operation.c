#include "secure_operation.h"
#include <limits.h>

int safe_add(int a, int b, int *out){ if(!out) return -1; if((b>0 && a>INT_MAX-b)||(b<0 && a<INT_MIN-b)) return -1; *out=a+b; return 0; }
int safe_sub(int a, int b, int *out){ if(!out) return -1; if((b>0 && a<INT_MIN+b)||(b<0 && a>INT_MAX+b)) return -1; *out=a-b; return 0; }
int safe_mul(int a, int b, int *out){ long long t; if(!out) return -1; t=(long long)a*(long long)b; if(t>INT_MAX||t<INT_MIN) return -1; *out=(int)t; return 0; }
int safe_div(int a, int b, int *out){ if(!out||b==0) return -1; if(a==INT_MIN&&b==-1) return -1; *out=a/b; return 0; }
