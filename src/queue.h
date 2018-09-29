#define define_list(type) \
struct _list_##type; \
typedef struct \
{ \
bool (*is_empty)(const struct _list_##type*); \
size_t (*size)(const struct _list_##type*); \
type (*pop_head)(struct _list_##type*); \
void (*push_tail)(struct _list_##type*, type); \
} _list_functions_##type; \
typedef struct _list_elem_##type \
{ \
type _data; \
struct _list_elem_##type* _next; \
} list_elem_##type; \
typedef struct _list_##type \
{ \
size_t _size; \
list_elem_##type* _first; \
list_elem_##type* _last; \
_list_functions_##type* _functions; \
} List_##type; \
List_##type* new_list_##type(); \
\
bool list_is_empty_##type(const List_##type* list); \
size_t list_size_##type(const List_##type* list); \
type list_pop_##type(List_##type* list);\
void list_push_##type(List_##type* list, type); \
\
bool list_is_empty_##type(const List_##type* list) \
{ \
return list->_size == 0; \
} \
\
size_t list_size_##type(const List_##type* list) \
{ \
return list->_size; \
} \
\
type list_pop_##type(List_##type* list) \
{ \
if(list->_size>0)\
{\
type temp = list->_first->_data;\
list_elem_##type* temp_p = list->_first;\
list->_first = list->_first->_next;\
free(temp_p);\
list->_size-=1;\
return temp;\
} else \
{\
printf("The queue is empty");\
exit(1);\
}\
} \
void list_push_##type(List_##type* list, type elem) \
{ \
list_elem_##type* new_ptr = (list_elem_##type*) malloc(sizeof(list_elem_##type));\
new_ptr->_data = elem;\
if(list->_size == 0)\
{\
list->_first = new_ptr;\
list->_last = new_ptr;\
} else \
{list->_last->_next = new_ptr ;\
list->_last = new_ptr;\
}\
list->_size+=1;\
} \
\
_list_functions_##type _list_funcs_##type = { \
&list_is_empty_##type, \
&list_size_##type, \
&list_pop_##type, \
&list_push_##type, \
};\
List_##type* new_list_##type() \
{\
List_##type* res = (List_##type*) malloc(sizeof(List_##type)); \
res->_size = 0; \
res->_first = NULL; \
res->_functions = &_list_funcs_##type; \
return res; \
}

#define List(type) List_##type
#define new_list(type) new_list_##type()

#define is_empty(collection) collection->_functions->is_empty(collection)
#define size(collection) collection->_functions->size(collection)
#define pop(collection) collection->_functions->pop_head(collection)
#define push(collection, elem) collection->_functions->push_tail(collection, elem)


