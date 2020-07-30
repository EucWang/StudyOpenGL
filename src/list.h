#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
//
//#include <stdlib.h>

/*单链表结构*/

typedef struct ListElmt_{
    void *data;
    struct ListElmt_ *next;
}ListElmt;

typedef struct List_{
    int size;
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    ListElmt *head;
    ListElmt *tail;
    ListElmt *current;
}List;

/*单链表结构的接口函数*/

/**
 * 链表初始化方法, 接收2个参数
 * @param list 需要被初始化的链表的指针
 * @param destroy 销毁数据的方法, 可以为NULL
 */
void list_init(List * list, void (* destroy)(void * data));

/**
*  销毁链表的方法
*/
void list_destroy(List *list);

/**
 * 在链表的指定元素之后插入一个新链表元素
 * @param list   需要插入元素的链表
 * @param elmt   链表中的存在的某一个链表元素结构, 可以为NULL, 如果为NULL, 则数据默认插入链表头部
 * @param data   需要插入到链表中的数据
 * @return   是否插入成功, -1: 失败, 0: 成功
 */
int list_ins_next(List *list, ListElmt *elmt, const void *data);

/**
 * 在链表尾部增加一个元素
 * @param list   需要插入元素的链表
 * @param data   需要插入到链表中的数据
 * @return   是否插入成功, -1: 失败, 0: 成功
 */
int list_add(List *list, const void *data);

/**
 * 在链表头部插入一个元素
 * @param list  需要插入元素的链表
 * @param data   需要插入到链表中的数据
 * @return   是否插入成功, -1: 失败, 0: 成功
 */
int list_add_first(List *list, const void *data);

/**
* 移除元素
*/
int list_rem_next(List *list, ListElmt *elmt, void **data);

/**
* 获得链表的长度
*/
#define list_size(list) ((list)->size)

/**
* 获得链表的头元素
*/
#define list_head(list) ((list)->head)

/**
* 获得链表的最后一个元素
*/
#define list_tail(list) ((list)->tail)

/**
* 判断元素是否是头元素
*/
#define list_is_head(list, elmt) ((list)->head == (elmt) ? 1:0)

/**
* 判断元素是否是尾元素
*/
#define list_is_tail(elmt) ((elmt)->next == NULL ?1:0)

#define list_data(elmt) ((elmt)->data)
#define list_next(elmt) ((elmt)->next)


/*遍历Set集合用到的几个函数*/

/*判断指示器是否有下一个元素*/
int list_hasNext(List *plist);

/*移动指示器到下一个位置*/
void list_moveToNext(List *plist);

/*获取指示器位置处的data数据*/
void list_iterator(List *plist, void **data);
/*重置指示器*/
void list_resetIterator(List *plist);

#endif // LIST_H_INCLUDED
