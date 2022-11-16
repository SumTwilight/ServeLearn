//******************************************************
//**************工具宏定义********************************
//******************************************************

//根据指针值删除内存
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)       if( (x)!=NULL ){ delete (x); (x)=NULL; }
#endif

//根据指针值删除数组类型内存
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if( (x)!=NULL ){ delete[] (x); (x)=NULL; }
#endif

//根据指针调用free接口
#ifndef SAFE_FREE
#define SAFE_FREE(x)         if( (x)!=NULL ){ free(x); (x)=NULL; }
#endif

//根据指针调用Release接口
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)      if( (x)!=NULL ){ (x)->Release(); (x)=NULL; }
#endif

#ifndef MAX
#define MAX(a,b)     ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef MIN
#define MIN(a,b)     ( ((a) < (b)) ? (a) : (b) )
#endif