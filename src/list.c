# pragma warning (disable:4819)
/* 单链表的实现 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "list.h"

/**
* 初始化链表
*/
void list_init(List *list, void (*destroy)(void *data)){
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
    return;
}

/**
* 销毁链表
*/
void list_destroy(List *list){
    void *data;
    while(list_size(list) > 0){
        if(list_rem_next(list, NULL, (void **)(&data)) == 0 &&
           list->destroy != NULL){
            list->destroy(data);
        }
    }
    memset(list, 0 , sizeof(List));
    return;
}

int list_add(List *list, const void *data) {
	return list_ins_next(list, list_tail(list), data);
}

int list_add_first(List *list, const void *data) {
	return list_ins_next(list, NULL, data);
}

int list_ins_next(List *list, ListElmt *elmt, const void *data){
    ListElmt *new_elmt;                               //声明一个新的链表元素
    new_elmt = (ListElmt *)malloc(sizeof(ListElmt));  //为这个新链表元素分配空间
    if(new_elmt == NULL){
    	printf("list_ins_next malloc err\n");
        return -1;                                    //分配的内存为NULL,则返回
    }
    new_elmt->data = (void *)data;                    //为新元素赋值

    if(elmt == NULL){                                 //第二个形参为NULL, 将新的data数据插入到列表头部
        if(list_size(list) == 0){
            list->tail = new_elmt;                    //如果链表为空, 则新链表元素就是 '链表尾'
        }
        new_elmt->next = list->head;                  // 新链表元素 设置为新的 '链表头'代替原来的'链表头'位置
        list->head = new_elmt;
    }else{                                           //第二个形参不为NULL,将新的data数据插入到其后面
        if(elmt->next == NULL){
            list->tail = new_elmt;             //如果第二个参数的下一个指针为NULL, 则 设置 '链表尾' 为 新链表元素
        }
        new_elmt->next = elmt->next;          // 将 新链表元素放置在第二个行参后面
        elmt->next = new_elmt;
    }

    list->size++;
    return 0;
}

int list_rem_next(List *list, ListElmt *elmt, void **data){
    if(list_size(list) == 0){
        return -1;
    }

    ListElmt *old_elmt;

    if(elmt == NULL){   //如果第二个形参为NULL,则将head元素删除
        old_elmt = list->head;
        *data = old_elmt->data;

        list->head = list->head->next;

        if(list_size(list) == 1){
            list->tail = NULL;
        }
    }else{           //如果第二个形参不为NULL,则将其后的元素删除
        if(elmt->next == NULL){
            return -1;
        }

        old_elmt = elmt->next;
        *data = old_elmt->data;
        elmt->next = elmt->next->next;

        if(elmt->next == NULL){
            list->tail = elmt;
        }
    }

    free(old_elmt);
    list->size--;
    return 0;
}


/**********************************************/

/*判断指示器是否有下一个元素*/
int list_hasNext(List *plist){
    if(plist->current == NULL){
        if(plist->head == NULL){
            return 0;
        }else{

            return 1;
        }
    }else{
        if(plist->current->next == NULL){
            return 0;
        }else{
            return 1;
        }
    }
}

/*移动指示器到下一个位置*/
void list_moveToNext(List *plist){
    if(plist->current == NULL){
    	plist->current = plist->head;
    }else{
    	plist->current = plist->current->next;
    }
}

/*获取指示器位置处的data数据*/
void list_iterator(List *plist, void **data){
        *data = plist->current->data;
}
/*重置指示器*/
void list_resetIterator(List *plist){
	plist->current = NULL;
}
/**********************************************/


