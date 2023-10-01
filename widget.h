#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QOpenGLWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void polyfill();
    typedef struct node{
      float current_x;//与当前扫面线的x交点坐标
      float delta_x;//每条扫描线的x增量
      int max_y;//本条线段的最大y坐标
      int index;
      struct node* next;//下一个交点结点
    }AET;

    typedef struct{
        int x;
        int y;
    }pointx_y;//顶点结构体
    typedef struct{
        int y_min;
        int y_max;
        int x_min;
        int x_max;
        float delta;
    }linex_y;//边结构体
    //void bubbleSort(pointx_y arr[], int n);//冒泡排序
    void insertNode(AET* &head, float current_x_egg, float delta_egg, int max_y_egg, int index_egg);
    void deleteNode(AET* &head,int max_y_egg);
    void traverse(AET* head);
    bool traverse_is_data(AET* head,int is_max_y);
    void insertInAscendingOrder(AET*& head, float current_x_egg, float delta_egg, int max_y_egg, int index_egg);
    bool is_horizontal(std::vector<int> m, int index);
private:
    Ui::Widget *ui;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};
#endif // WIDGET_H
