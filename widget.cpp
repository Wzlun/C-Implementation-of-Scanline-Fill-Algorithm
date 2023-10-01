#include "widget.h"
#include "ui_widget.h"
#include <QOpenGLFunctions>
#include <GL\glu.h>
#include <cmath>
#include <QDebug>
#include <vector>
#define MAX 4
Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 设置OpenGL窗口的大小，这里设置为宽度为800像素，高度为600像素
    this->setFixedSize(500, 500);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::initializeGL()
{

}
void Widget::resizeGL(int w, int h)
{
        glViewport(0, 0, w, h);
}
void Widget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 401.0, -1.0, 401.0); //窗口坐标范围

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glFlush();//用于强制刷新缓冲，保证绘图命令将被执行
    polyfill();
}
void Widget::polyfill()
{
    /*---------------五角星示例---------------*/
//    pointx_y Point[MAX]={{181,306},{144,199},{20,199},{120,133},{82,26},
//                        {181,90},{280,26},{242,133},{342,199},{218,199}
//                        };
    /*---------------任意六边形示例---------------*/
//   pointx_y Point[MAX]={{252,290},{141,101},{190,273},{152,398},{240,308},{367,247}};
    /*---------------四边形示例---------------*/
    pointx_y Point[MAX]={{100,200},{100,100},{200,100},{160,160}};
    /*---------------正方形示例---------------*/
//    pointx_y Point[MAX]={{100,200},{100,100},{200,100},{200,200}};
    std::vector<int> v;//定义一个存放水平线索引的容器
    float slope[MAX] = {0};
    for(int j=0;j<=MAX-1;j++)//求斜率
    {
        slope[j%MAX] = (float)(Point[j%MAX].x-Point[(j+1)%MAX].x)/(Point[j%MAX].y-Point[(j+1)%MAX].y);
    }

    linex_y Line[MAX];//定义边结构体类型的变量
    for(int i=0;i<=MAX-1;i++)
    {
        if(Point[i].y<=Point[(i+1)%MAX].y)
        {
            if(Point[i].x<=Point[(i+1)%MAX].x)
            {
                Line[i].x_min=Point[i].x;
                Line[i].x_max=Point[(i+1)%MAX].x;
            }
            else
            {
                Line[i].x_max=Point[i].x;
                Line[i].x_min=Point[(i+1)%MAX].x;
            }
            if(Point[i].y>=Point[(i+1)%MAX].y)
            {
                Line[i].y_max=Point[i].y;
                Line[i].y_min=Point[(i+1)%MAX].y;
            }
            else
            {
                Line[i].y_min=Point[i].y;
                Line[i].y_max=Point[(i+1)%MAX].y;
            }
        }
        else
        {
            if(Point[i].x<=Point[(i+1)%MAX].x)
            {
                Line[i].x_min=Point[i].x;
                Line[i].x_max=Point[(i+1)%MAX].x;
            }
            else
            {
                Line[i].x_max=Point[i].x;
                Line[i].x_min=Point[(i+1)%MAX].x;
            }
            if(Point[i].y>=Point[(i+1)%MAX].y)
            {
                Line[i].y_max=Point[i].y;
                Line[i].y_min=Point[(i+1)%MAX].y;
            }
            else
            {
                Line[i].y_min=Point[i].y;
                Line[i].y_max=Point[(i+1)%MAX].y;
            }
        }
        Line[i].delta=slope[i];
    }
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX-1; j++) {
            if (Point[j].x > Point[j + 1].x) {
                // 交换arr[j]和arr[j+1]
                pointx_y temp = {Point[j].x,Point[j].y};
                Point[j] = Point[j + 1];
                Point[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX-1; j++) {
            float x_b=(Line[j].x_max+Line[j].x_min)/2;
            float x_1_b=(Line[j+1].x_max+Line[j+1].x_min)/2;
            if (x_b>=x_1_b) {
                // 交换arr[j]和arr[j+1]
                linex_y temp = Line[j];
                Line[j]=Line[j+1];
                Line[j + 1] = temp;
            }
        }
    }
    for(int i = 0;i<MAX;i++)
    {
        if(Line[i].y_max == Line[i].y_min)
        {
            v.push_back(i);//如果为水平线，就将该边的索引放入容器
        }
    }

//截至到此已经将边按照x的升序存在结构体数组Line里面，将各点按照x的升序存在数组Point中
    int min_y=Point[0].y;
    int max_y=Point[0].y;
    for(int i=0;i<=MAX-1;i++)
    {
        if(Point[i].y<=min_y)
        {
            min_y = Point[i].y;
        }
        if(Point[i].y>=max_y)
        {
            max_y = Point[i].y;
        }
    }
/******************************************************创建新边表****************************************************/
    AET** NET = (AET**)malloc(400*sizeof(AET*));//创建头结点
    for (int i = 0; i <= max_y; i++)
    {
            NET[i] = NULL; // 初始化每个指针为空
    }//创建新边表的表头链表
    for(int i= min_y;i<=max_y;i++)
    {
        for(int j=0;j<=MAX-1;j++)
        {
            if(Line[j].y_min==i)
            {
                if(Line[j].delta>0)
                {
                    insertNode(NET[i], Line[j].x_min, Line[j].delta, Line[j].y_max, j);
                }
                else
                {
                    insertNode(NET[i], Line[j].x_max, Line[j].delta, Line[j].y_max, j);
                }
            }
        }
    }
   //traverse(NET[368]);
/******************************************************创建活动边表****************************************************/
    AET* AET_Live = (AET*)malloc(sizeof(AET));//创建活动边表头结点
    AET_Live = NULL;
    AET* top = NET[min_y];
    while(top!=NULL)
    {
        if(!is_horizontal(v, top->index))//舍弃水平边
        {
            insertNode(AET_Live, top->current_x, top->delta_x,top->max_y, top->index);
        }
        top=top->next;
    }

    int scan = min_y+1;

    while(scan<max_y)
    {
        if(traverse_is_data(AET_Live,scan))//如果遍历活动边表存在已经到达终点的边，将该边结点删除
        {
            deleteNode(AET_Live,scan);
        }
        if(NET[scan]!=NULL)//如果此时刚好有新边加入
        {
            AET* top = NET[scan];
            while(top!=NULL)
            {
                if(!is_horizontal(v, top->index))//舍弃水平边
                {
                    insertInAscendingOrder(AET_Live, top->current_x, top->delta_x, top->max_y, top->index);
                }
                top = top->next;
            }
        }
        AET* odd = AET_Live;//创建一个指针指向奇数结点
        AET* event = AET_Live;//创建一个指针指向偶数结点
        odd = AET_Live;
        event = AET_Live->next;

        while(event!=NULL)//循环条件是偶指针不为空
        {
            glBegin(GL_LINES);
                    // 指定第一个点的坐标
                    glVertex2f(odd->current_x,scan);//第一个点是第一个奇指针指向的x
                    // 指定第二个点的坐标
                    glVertex2f(event->current_x, scan);//第二个点是第一个偶指针指向的x

                    odd->current_x=odd->current_x+odd->delta_x;
                    event->current_x=event->current_x+event->delta_x;//奇偶指针的x分别增加delta
            glEnd();
            glFlush();
            if(event->next == NULL)
            {
                break;
            }
            odd = event->next;
            event = odd->next;//奇偶指针分别向后移动两个单位绘制另外线
        }

        scan++;
    }
}

void Widget::insertNode(AET* &head, float current_x_egg, float delta_egg, int max_y_egg, int index_egg)
{
    // 创建新节点
    AET* newnode = (AET*)malloc(sizeof(AET));
    newnode->current_x = current_x_egg;
    newnode->delta_x = delta_egg;
    newnode->max_y = max_y_egg;
    newnode->index = index_egg;
    newnode->next = NULL; // 新节点的next指向nullptr，表示尾部
    if (head == NULL) {
        // 如果链表为空，将新节点作为头节点
        head = newnode;
    } else {
        // 否则，找到链表尾部，将新节点连接到尾部
        AET* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newnode;
    }
}
void Widget::traverse(AET* head)
{
    if(head==NULL)
    {
        return;
    }
    qDebug()<<head->max_y;
    traverse(head->next);
}


void Widget::deleteNode(AET* &head,int target)
{
    AET* p = NULL;
    AET* q = NULL;
    p = head->next;
    q = head;
    while(p!=NULL)
    {
        if(p->max_y == target)
        {
            q->next = p->next;
            free(p);
            p = q->next;
        }
        else
        {
            q = p;
            p = p->next;
        }
    }
    if(head->max_y == target)
    {
        q = head;
        head = head->next;
        free(q);
    }
}



bool Widget::traverse_is_data(AET* head,int is_max_y)
{
    AET* top = head;
    while(top!=NULL)
    {
        if(top->max_y == is_max_y)
        {
            return true;
        }
        else
        {
            top = top->next;
        }
    }
    return false;
}


void Widget::insertInAscendingOrder(AET*& head, float current_x_egg, float delta_egg, int max_y_egg, int index_egg)
{
    AET* newnode = (AET*)malloc(sizeof(AET));
    newnode->current_x = current_x_egg;
    newnode->delta_x = delta_egg;
    newnode->max_y = max_y_egg;
    newnode->index = index_egg;
    // 如果链表为空或插入节点值小于头节点的值，将新节点作为头节点
    if (head == nullptr || index_egg < head->index) {
        newnode->next = head;
        head = newnode;
        return;
    }

    // 遍历链表找到插入位置
    AET* current = head;
    while (current->next != nullptr && current->next->index < index_egg) {
        current = current->next;
    }

    // 插入新节点
    newnode->next = current->next;
    current->next = newnode;
}
bool Widget::is_horizontal(std::vector<int> m, int index)
{
    for(std::vector<int>::iterator it = m.begin(); it!=m.end(); ++it)
    {
        if(index == *it)
        {
            return true;
        }
    }
    return false;
}
