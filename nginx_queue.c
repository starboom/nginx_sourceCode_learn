#include <stdio.h>

typedef struct ngx_queue_s ngx_queue_t;

struct ngx_queue_s{
	ngx_queue_t *prev;//前驱指针
	ngx_queue_t *next;//后驱指针
};

typedef
struct my_queue{
	char* str;
	ngx_queue_t que;
	int num;
}my_queue;//测试所用数据结构,
//包含一个ngx_queue_t类型的双向链表用来将数据连接起来

#define ngx_queue_init(q) 				\
	(q) -> prev = q;					\
	(q) -> next = q		//初始化双向链表

#define ngx_queue_empty(h) 				\
	(h == (h) -> prev)	//清空双向链表节点

#define ngx_queue_insert_head(h,x) 		\
	(x) -> next = (h) -> next;			\
	(x) -> next -> prev = x;			\
	(x) -> prev = h;					\
	(h) -> next = x //h 是 初始的双链表头

#define ngx_queue_insert_after ngx_queue_insert_head

#define ngx_queue_insert_tail(h,x) 	\
	(x) -> prev = (h) -> prev;		\
	(x) -> prev -> next = x;		\
	(x) -> next = h;				\
	(h) -> prev = x;

#define ngx_queue_head(h) \
	(h) -> next

#define ngx_queue_last(h) \
	(h) -> prev

#define ngx_queue_sentinel(h) \
	(h)

#define ngx_queue_next(q) \
	(q) -> next

#define ngx_queue_prev(q) \
	(q) -> prev

#define ngx_queue_remove(x) \
	(x) -> next -> prev = (x) -> prev; \
	(x) -> prev -> next = (x) -> next

#define ngx_queue_split(h,q,n) \
	(n) -> prev = (h) -> prev; \
	(n) -> prev -> next = n; \
	(n) -> next = q; \
	(h) -> prev = (q) -> prev; \
	(h) -> prev -> next = h; \
	(q) -> prev = n  //拆分链表，h是链表容器，q是链表h的一个元素。\
	//这个方法将链表h以元素q为界拆分成两个链表h和n，其中h有原链表的
	//前半部分收成(不包括q),而n有原链表的后半部分构成，q是它的首元素

#define ngx_queue_add(h,n) \
	(h) -> prev -> next = (n) -> next; \
	(n) -> next -> prev = (h) -> prev; \ 
	(h) -> prev = (n) -> prev; \
	(h) -> prev -> next = h;
//offsetof这个宏可以用来表示一个数据结构里的成员对于这个数据结构来说
//的偏移量
#define offsetof(type,member) (size_t)&(((type*)0) -> member)

#define ngx_queue_data(q,type,link) \
	(type *)((char*) q - offsetof(type,link))

/******************************************************************/

ngx_queue_t *ngx_queue_middle(ngx_queue_t *queue);


void ngx_queue_sort(ngx_queue_t *queue,
					int (*cmp)(const ngx_queue_t*,
					const ngx_queue_t *)){

ngx_queue_t  *q, *prev, *next;

q = ngx_queue_head(queue);

if (q == ngx_queue_last(queue)){
    return;
}

for (q = ngx_queue_next(q); q != ngx_queue_sentinel(queue); q = next){

    prev = ngx_queue_prev(q);
    next = ngx_queue_next(q);

    ngx_queue_remove(q);

    do {
        if (cmp(prev, q) <= 0) {
            break;
        }

        prev = ngx_queue_prev(prev);

    } while (prev != ngx_queue_sentinel(queue));

    ngx_queue_insert_after(prev, q);
}

}

int compTestNode(const ngx_queue_t* a,const ngx_queue_t* b){
	/*首先使用ngx_queue_data方法由ngx_queue_t变量获取元素结构体
	TestNode的地址*/
	my_queue* aNode = ngx_queue_data(a,my_queue,que);
	my_queue* bNode = ngx_queue_data(b,my_queue,que);

	return aNode -> num > bNode -> num;
}

int main(int argc, char const *argv[])
{
	
	ngx_queue_t queueContainer;
	ngx_queue_init(&queueContainer);

	int i = 0;
	my_queue node[5];
	for(;i < 5;++i){
		node[i].num = i;
	}
	ngx_queue_insert_tail(&queueContainer,&node[0].que);
	ngx_queue_insert_head(&queueContainer,&node[1].que);
	ngx_queue_insert_tail(&queueContainer,&node[2].que);
	ngx_queue_insert_after(&queueContainer,&node[3].que);
	ngx_queue_insert_tail(&queueContainer,&node[4].que);

	ngx_queue_sort(&queueContainer,compTestNode);

	ngx_queue_t *q;
	for(q = ngx_queue_head(&queueContainer);q != ngx_queue_sentinel(&queueContainer)
		;q = ngx_queue_next(q)){
		my_queue* eleNode = ngx_queue_data(q,my_queue,que);
		printf("%d\n",eleNode -> num );
	}


	
	return 0;
}
