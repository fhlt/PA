#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#include <stdlib.h>
#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].expr = NULL;
		wp_pool[i].value = 0;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP *get_head()
{
	return head;
}
void get_free_()
{
	WP *wp=free_;
	while(wp)
	{
		printf("%d\t",wp->NO);
		wp = wp->next;
	}	
	printf("\n");
}
/*最简单的方式将新的监视点插入*/
void add_to_head(WP *wp)
{
	wp->next = head;
	head = wp;
}

void renew(WP *wp)
{
//	printf("renew wp\n");
	WP *twp=NULL,*preWp=NULL;
	if(free_ == NULL)
	{
		free_ = wp;
		return ;
	}
	twp = preWp = free_;
	if(twp->NO > wp->NO)
	{
		wp->next = free_;
		free_ = wp;
		return;
	}
	preWp = twp;
	twp = twp->next;
	while(twp)
	{
		if((wp->NO > preWp->NO) && (wp->NO < twp->NO))
		{
			break;
		}
		preWp = twp;
		twp = twp->next;
	}
//	printf("跳出循环\n");
	//插入链表末尾
	if(twp == NULL)
	{
//		printf("twp ==null");
		preWp->next = wp;	
		wp->next  = NULL;
	}
	//中间
	else
	{
		preWp->next = wp;
		wp->next = twp;
	}
}
void free_wp(int no)
{
	WP *wp = NULL,*preWp = NULL;
	wp = preWp = head;
	while(wp)
	{
		if(wp->NO == no)
			break;
		preWp = wp;
		wp = wp->next;
	}
//	printf("检索完毕\n");
	if(wp == NULL)
	{
		printf("the %d is not be used\n",no);
		return ;
	}
	if(wp == head)
	{
//		printf("hell0\n");
		head = wp->next;
		free(wp->expr);
		wp->value = 0;
		wp->next = NULL;
		renew(wp);
	}
	else
	{	
		preWp->next = wp->next;
		free(wp->expr);
		wp->value = 0;
		wp->next = NULL;
		renew(wp);
	}
//	printf("delete %d\n",n);	
}

WP* new_wp()
{
	WP *wp=NULL;
	if(free_ == NULL)
	{
	//	printf("空闲监视点为空，失败！\n");
		return NULL;
	}
	wp = free_;
	free_ = free_->next;
	return wp;
}

