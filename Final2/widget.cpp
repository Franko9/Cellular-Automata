#include "widget.h"
#include "ui_widget.h"
#include "time.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    srand(time(0));

    settingGrid();
    initailizeVec();
    randomColors();

    myTimer->setInterval(1000.0/frames); //20 frames per second

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QColor colorOne = QColorDialog::getColor();
    QColor colorTwo = QColorDialog::getColor();

    startGradient(colorOne, colorTwo);
}

void Widget::startGradient(QColor start, QColor end)
{
    colorVec.clear();

    float rDiff, gDiff, bDiff, aDiff;

    QColor nextColor;

    rDiff = ((end.redF() - start.redF())*255)/(steps-1);
    gDiff = ((end.greenF() - start.greenF())*255)/(steps-1);
    bDiff = ((end.blueF() - start.blueF())*255)/(steps-1);
    aDiff = ((end.alphaF() - start.alphaF())*255)/(steps-1);

    for(int i = 0; i < steps; i++)
    {
        nextColor = QColor(start.red() + (rDiff*i), start.green() + (gDiff*i), start.blue() + bDiff*i, start.alpha() + (aDiff*i));
        colorVec.append(nextColor);
    }

    randomColors();
    repaint();

}

void Widget::addGradient(QColor next)
{
    float rDiff, gDiff, bDiff, aDiff;

    QColor nextColor;

    rDiff = ((next.redF() - colorVec.back().redF())*255)/(stepsMore);
    gDiff = ((next.greenF() - colorVec.back().greenF())*255)/(stepsMore);
    bDiff = ((next.blueF() - colorVec.back().blueF())*255)/(stepsMore);
    aDiff = ((next.alphaF() - colorVec.back().alphaF())*255)/(stepsMore);

    for(int i = 0; i < stepsMore; i++)
    {
        nextColor = QColor((colorVec.back().redF()*255) + rDiff, (colorVec.back().greenF()*255) + gDiff, (colorVec.back().blueF()*255) + bDiff, (colorVec.back().alphaF()*255) + aDiff);
        colorVec << nextColor;
    }

    randomColors();
    repaint();

}

void Widget::initailizeVec()
{
    startGradient(QColor(255, 255, 0), QColor(255, 165, 0));
    addGradient(QColor(255, 0, 0));
    addGradient(QColor(0, 0, 0, 255));

}


void Widget::randomColors()
{
    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            grid[i*rows + j] = rand() % (colorVec.size());
        }
    }
}

void Widget::settingGrid()
{
    delete [] grid;
    delete [] nextGrid;

    grid = new int[(height+10) * (height+10)];
    nextGrid = new int[(height+10) * (height+10)];

    memset(grid, 0, sizeof(int)*(height+10) * (height+10));
    memset(nextGrid, 0, sizeof(int)*(height+10) * (height+10));

    rows = height;
    cols = height;

    cellWidth = 500.0/height;
    cellHeight = 500.0/height;
}

void Widget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    drawRectangles(p);

    count++;
    qDebug() << count;
}

void Widget::drawRectangles(QPainter &p)
{
    QRectF rectangle(200, 200, cellWidth, cellWidth);
    QBrush myBrush;

    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            rectangle.moveRight((j-5)*cellWidth + 20);
            rectangle.moveTop((i-5)*cellWidth + 110);

            p.fillRect(rectangle, colorVec[grid[i*rows + j]]);
        }
    }
}

void Widget::run()
{
    int mod;

    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            mod = 0;

            if(grid[(i+1)*height + j] - grid[i*height + j] == 1|| grid[(i+1)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-1)*height + j] - grid[i*height + j] == 1 || grid[(i-1)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[i*height + (j+1)] - grid[i*height + j] == 1|| grid[i*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[i*height + (j-1)] - grid[i*height + j] == 1|| grid[i*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }


            if(grid[(i+1)*height + (j+1)] - grid[i*height + j] == 1|| grid[(i+1)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-1)*height + (j+1)] - grid[i*height + j] == 1 || grid[(i-1)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+1)*height + (j-1)] - grid[i*height + j] == 1|| grid[i*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-1)*height + (j-1)] - grid[i*height + j] == 1|| grid[i*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }


            if(mod >= threshold)
            {
                nextGrid[i*height + j] = grid[i*height + j] + 1;

                if(nextGrid[i*height + j] > colorVec.size()-1)
                {
                    nextGrid[i*height + j] = 0;
                }
            }
            else
            {
                nextGrid[i*height + j] = grid[i*height + j];
            }

        }
    }

    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            grid[i*height + j] = nextGrid[i*height + j];
        }
    }

    update();
}

void Widget::run2()
{
    int mod;

    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            mod = 0;

            if(grid[(i+1)*height + j] - grid[i*height + j] == 1|| grid[(i+1)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-1)*height + j] - grid[i*height + j] == 1 || grid[(i-1)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[i*height + (j+1)] - grid[i*height + j] == 1|| grid[i*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[i*height + (j-1)] - grid[i*height + j] == 1|| grid[i*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }


            if(grid[(i+1)*height + (j+1)] - grid[i*height + j] == 1|| grid[(i+1)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-1)*height + (j+1)] - grid[i*height + j] == 1 || grid[(i-1)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+1)*height + (j-1)] - grid[i*height + j] == 1|| grid[i*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-1)*height + (j-1)] - grid[i*height + j] == 1|| grid[i*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }


            if(grid[(i+2)*height + (j-2)] - grid[i*height + j] == 1|| grid[(i+2)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+2)*height + (j-1)] - grid[i*height + j] == 1 || grid[(i+2)*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+2)*height + j] - grid[i*height + j] == 1|| grid[(i+2)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+2)*height + (j+1)] - grid[i*height + j] == 1|| grid[(i+2)*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+2)*height + (j+2)] - grid[i*height + j] == 1|| grid[(i+2)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }



            if(grid[(i-2)*height + (j-2)] - grid[i*height + j] == 1|| grid[(i-2)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-2)*height + (j-1)] - grid[i*height + j] == 1 || grid[(i-2)*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-2)*height + j] - grid[i*height + j] == 1|| grid[(i-2)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-2)*height + (j+1)] - grid[i*height + j] == 1|| grid[(i-2)*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-2)*height + (j+2)] - grid[i*height + j] == 1|| grid[(i-2)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }


            if(grid[(i+1)*height + (j-2)] - grid[i*height + j] == 1|| grid[(i+1)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+1)*height + (j+2)] - grid[i*height + j] == 1 || grid[(i+1)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[i*height + (j-2)] - grid[i*height + j] == 1|| grid[i*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[i*height + (j+2)] - grid[i*height + j] == 1|| grid[i*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-1)*height + (j+2)] - grid[i*height + j] == 1 || grid[(i-1)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-1)*height + (j-2)] - grid[i*height + j] == 1|| grid[(i-1)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }


            if(mod >= threshold)
            {
                nextGrid[i*height + j] = grid[i*height + j] + 1;

                if(nextGrid[i*height + j] > colorVec.size()-1)
                {
                    nextGrid[i*height + j] = 0;
                }
            }
            else
            {
                nextGrid[i*height + j] = grid[i*height + j];
            }
        }
    }

    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            grid[i*height + j] = nextGrid[i*height + j];
        }
    }

    update();
}

void Widget::run3()
{
    int mod;

    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            mod = 0;

            if(grid[(i+2)*height + (j-2)] - grid[i*height + j] == 1|| grid[(i+2)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+2)*height + (j-1)] - grid[i*height + j] == 1 || grid[(i+2)*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+2)*height + j] - grid[i*height + j] == 1|| grid[(i+2)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+2)*height + (j+1)] - grid[i*height + j] == 1|| grid[(i+2)*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+2)*height + (j+2)] - grid[i*height + j] == 1|| grid[(i+2)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }



            if(grid[(i-2)*height + (j-2)] - grid[i*height + j] == 1|| grid[(i-2)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-2)*height + (j-1)] - grid[i*height + j] == 1 || grid[(i-2)*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-2)*height + j] - grid[i*height + j] == 1|| grid[(i-2)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-2)*height + (j+1)] - grid[i*height + j] == 1|| grid[(i-2)*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-2)*height + (j+2)] - grid[i*height + j] == 1|| grid[(i-2)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }


            if(grid[(i+1)*height + (j-2)] - grid[i*height + j] == 1|| grid[(i+1)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i+1)*height + (j+2)] - grid[i*height + j] == 1 || grid[(i+1)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[i*height + (j-2)] - grid[i*height + j] == 1|| grid[i*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[i*height + (j+2)] - grid[i*height + j] == 1|| grid[i*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-1)*height + (j+2)] - grid[i*height + j] == 1 || grid[(i-1)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }
            if(grid[(i-1)*height + (j-2)] - grid[i*height + j] == 1|| grid[(i-1)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
            {
                mod++;
            }


            if(mod >= threshold)
            {
                nextGrid[i*height + j] = grid[i*height + j] + 1;

                if(nextGrid[i*height + j] > colorVec.size()-1)
                {
                    nextGrid[i*height + j] = 0;
                }
            }
            else
            {
                nextGrid[i*height + j] = grid[i*height + j];
            }
        }
    }

    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            grid[i*height + j] = nextGrid[i*height + j];
        }
    }

    update();
}

void Widget::run4()
{
    int mod;

    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            mod = 0;

            if(squares[0][0])
                if(grid[(i-3)*height + (j-3)] - grid[i*height + j] == 1 || grid[(i-3)*height + (j-3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[0][1])
                if(grid[(i-3)*height + (j-2)] - grid[i*height + j] == 1 || grid[(i-3)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[0][2])
                if(grid[(i-3)*height + (j-1)] - grid[i*height + j] == 1 || grid[(i-3)*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[0][3])
                if(grid[(i-3)*height + j] - grid[i*height + j] == 1 || grid[(i-3)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[0][4])
                if(grid[(i-3)*height + (j+1)] - grid[i*height + j] == 1 || grid[(i-3)*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[0][5])
                if(grid[(i-3)*height + (j+2)] - grid[i*height + j] == 1 || grid[(i-3)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[0][6])
                if(grid[(i-3)*height + (j+3)] - grid[i*height + j] == 1 || grid[(i-3)*height + (j+3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }




            if(squares[1][0])
                if(grid[(i-2)*height + (j-3)] - grid[i*height + j] == 1 || grid[(i-2)*height + (j-3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[1][1])
                if(grid[(i-2)*height + (j-2)] - grid[i*height + j] == 1 || grid[(i-2)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[1][2])
                if(grid[(i-2)*height + (j-1)] - grid[i*height + j] == 1 || grid[(i-2)*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[1][3])
                if(grid[(i-2)*height + j] - grid[i*height + j] == 1 || grid[(i-2)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[1][4])
                if(grid[(i-2)*height + (j+1)] - grid[i*height + j] == 1 || grid[(i-2)*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[1][5])
                if(grid[(i-2)*height + (j+2)] - grid[i*height + j] == 1 || grid[(i-2)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[1][6])
                if(grid[(i-2)*height + (j+3)] - grid[i*height + j] == 1 || grid[(i-2)*height + (j+3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }




            if(squares[2][0])
                if(grid[(i-1)*height + (j-3)] - grid[i*height + j] == 1 || grid[(i-1)*height + (j-3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[2][1])
                if(grid[(i-1)*height + (j-2)] - grid[i*height + j] == 1 || grid[(i-1)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[2][2])
                if(grid[(i-1)*height + (j-1)] - grid[i*height + j] == 1 || grid[(i-1)*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[2][3])
                if(grid[(i-1)*height + j] - grid[i*height + j] == 1 || grid[(i-1)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[2][4])
                if(grid[(i-1)*height + (j+1)] - grid[i*height + j] == 1 || grid[(i-1)*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[2][5])
                if(grid[(i-1)*height + (j+2)] - grid[i*height + j] == 1 || grid[(i-1)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[2][6])
                if(grid[(i-1)*height + (j+3)] - grid[i*height + j] == 1 || grid[(i-1)*height + (j+3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }





            if(squares[3][0])
                if(grid[i*height + (j-3)] - grid[i*height + j] == 1 || grid[i*height + (j-3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[3][1])
                if(grid[i*height + (j-2)] - grid[i*height + j] == 1 || grid[i*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[3][2])
                if(grid[i*height + (j-1)] - grid[i*height + j] == 1 || grid[i*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }


            if(squares[3][4])
                if(grid[i*height + (j+1)] - grid[i*height + j] == 1 || grid[i*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[3][5])
                if(grid[i*height + (j+2)] - grid[i*height + j] == 1 || grid[i*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[3][6])
                if(grid[i*height + (j+3)] - grid[i*height + j] == 1 || grid[i*height + (j+3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }





            if(squares[4][0])
                if(grid[(i+1)*height + (j-3)] - grid[i*height + j] == 1 || grid[(i+1)*height + (j-3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[4][1])
                if(grid[(i+1)*height + (j-2)] - grid[i*height + j] == 1 || grid[(i+1)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[4][2])
                if(grid[(i+1)*height + (j-1)] - grid[i*height + j] == 1 || grid[(i+1)*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[4][3])
                if(grid[(i+1)*height + j] - grid[i*height + j] == 1 || grid[(i+1)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[4][4])
                if(grid[(i+1)*height + (j+1)] - grid[i*height + j] == 1 || grid[(i+1)*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[4][5])
                if(grid[(i+1)*height + (j+2)] - grid[i*height + j] == 1 || grid[(i+1)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[4][6])
                if(grid[(i+1)*height + (j+3)] - grid[i*height + j] == 1 || grid[(i+1)*height + (j+3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }




            if(squares[5][0])
                if(grid[(i+2)*height + (j-3)] - grid[i*height + j] == 1 || grid[(i+2)*height + (j-3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[5][1])
                if(grid[(i+2)*height + (j-2)] - grid[i*height + j] == 1 || grid[(i+2)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[5][2])
                if(grid[(i+2)*height + (j-1)] - grid[i*height + j] == 1 || grid[(i+2)*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[5][3])
                if(grid[(i+2)*height + j] - grid[i*height + j] == 1 || grid[(i+2)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[5][4])
                if(grid[(i+2)*height + (j+1)] - grid[i*height + j] == 1 || grid[(i+2)*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[5][5])
                if(grid[(i+2)*height + (j+2)] - grid[i*height + j] == 1 || grid[(i+2)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[5][6])
                if(grid[(i+2)*height + (j+3)] - grid[i*height + j] == 1 || grid[(i+2)*height + (j+3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }


            if(squares[6][0])
                if(grid[(i+3)*height + (j-3)] - grid[i*height + j] == 1 || grid[(i+3)*height + (j-3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[6][1])
                if(grid[(i+3)*height + (j-2)] - grid[i*height + j] == 1 || grid[(i+3)*height + (j-2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[6][2])
                if(grid[(i+3)*height + (j-1)] - grid[i*height + j] == 1 || grid[(i+3)*height + (j-1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[6][3])
                if(grid[(i+3)*height + j] - grid[i*height + j] == 1 || grid[(i+3)*height + j] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[6][4])
                if(grid[(i+3)*height + (j+1)] - grid[i*height + j] == 1 || grid[(i+3)*height + (j+1)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[6][5])
                if(grid[(i+3)*height + (j+2)] - grid[i*height + j] == 1 || grid[(i+3)*height + (j+2)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(squares[6][6])
                if(grid[(i+3)*height + (j+3)] - grid[i*height + j] == 1 || grid[(i+3)*height + (j+3)] - grid[i*height + j] == -(colorVec.size()-1))
                {
                    mod++;
                }

            if(mod >= threshold)
            {
                nextGrid[i*height + j] = grid[i*height + j] + 1;

                if(nextGrid[i*height + j] > colorVec.size()-1)
                {
                    nextGrid[i*height + j] = 0;
                }
            }
            else
            {
                nextGrid[i*height + j] = grid[i*height + j];
            }
        }
    }

    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            grid[i*height + j] = nextGrid[i*height + j];
        }
    }

    update();
}

void Widget::run5()
{
    int val;

    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            val = rand()%4;

            if(val == 0 && grid[i*height + (j-1)] < 100)
            {
                nextGrid[i*height + j] = grid[i*height + (j-1)];
            }
            else if(val == 1 && grid[i*height + (j+1)] < 100)
            {
                nextGrid[i*height + j] = grid[i*height + (j+1)];
            }
            else if(val == 2 && grid[(i-1)*height + j] < 100)
            {
                nextGrid[i*height + j] = grid[(i-1)*height + j];
            }
            else if(val == 3 && grid[(i+1)*height + j] < 100)
            {
                nextGrid[i*height + j] = grid[(i+1)*height + j];
            }
        }
    }

    for(int i = 5; i < rows+5; i++)
    {
        for(int j = 5; j < cols+5; j++)
        {
            grid[i*height + j] = nextGrid[i*height + j];
        }
    }

    update();
}


void Widget::on_spinBox_valueChanged(int arg1)
{
    steps = arg1;
}

void Widget::on_pushButton_2_clicked()
{
    QColor color = QColorDialog::getColor();

    addGradient(color);
}

void Widget::on_spinBox_2_valueChanged(int arg1)
{
    stepsMore = arg1;
}

void Widget::on_pushButton_3_clicked()
{
    if(automataTypeTwo == 0)
    {
        if(automataType == 0)
        {
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run2()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run3()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run4()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run5()));
            connect(myTimer, SIGNAL(timeout()), this, SLOT(run()));
        }
        if(automataType == 1)
        {
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run3()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run4()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run5()));
            connect(myTimer, SIGNAL(timeout()), this, SLOT(run2()));
        }
        if(automataType == 2)
        {
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run2()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run4()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run5()));
            connect(myTimer, SIGNAL(timeout()), this, SLOT(run3()));
        }
        if(automataType == 3)
        {
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run2()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run3()));
            disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run5()));
            connect(myTimer, SIGNAL(timeout()), this, SLOT(run4()));
        }
    }
    if(automataTypeTwo == 1)
    {
        disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run()));
        disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run2()));
        disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run3()));
        disconnect(myTimer, SIGNAL(timeout()), this, SLOT(run4()));
        connect(myTimer, SIGNAL(timeout()), this, SLOT(run5()));
    }
    myTimer->start();
}

void Widget::on_pushButton_4_clicked()
{
    myTimer->stop();
}



void Widget::on_comboBox_activated(int index)
{
    automataType = index;
}

void Widget::on_pushButton_6_clicked()
{
    randomColors();
    update();
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    height = value;
    settingGrid();
    randomColors();
    update();
}



void Widget::on_horizontalSlider_2_valueChanged(int value)
{
    frames = value;
    myTimer->setInterval(1000.0/frames);
}

void Widget::on_checkBox_00_stateChanged(int arg1)
{
    squares[0][0] = arg1;
    qDebug() << squares[0][0];
}
void Widget::on_checkBox_01_stateChanged(int arg1)
{
    squares[0][1] = arg1;
    qDebug() << squares[0][1];
}
void Widget::on_checkBox_02_stateChanged(int arg1)
{
    squares[0][2] = arg1;
    qDebug() << squares[0][2];

}
void Widget::on_checkBox_03_stateChanged(int arg1)
{
    squares[0][3] = arg1;
    qDebug() << squares[0][3];

}
void Widget::on_checkBox_04_stateChanged(int arg1)
{
    squares[0][4] = arg1;
    qDebug() << squares[0][4];

}
void Widget::on_checkBox_05_stateChanged(int arg1)
{
    squares[0][5] = arg1;
    qDebug() << squares[0][5];

}
void Widget::on_checkBox_06_stateChanged(int arg1)
{
    squares[0][6] = arg1;
    qDebug() << squares[0][6];
}

void Widget::on_checkBox_10_stateChanged(int arg1)
{
    squares[1][0] = arg1;
    qDebug() << squares[1][0];
}
void Widget::on_checkBox_11_stateChanged(int arg1)
{
    squares[1][1] = arg1;
    qDebug() << squares[1][1];
}
void Widget::on_checkBox_12_stateChanged(int arg1)
{
    squares[1][2] = arg1;
    qDebug() << squares[1][2];
}
void Widget::on_checkBox_13_stateChanged(int arg1)
{
    squares[1][3] = arg1;
    qDebug() << squares[1][3];
}
void Widget::on_checkBox_14_stateChanged(int arg1)
{
    squares[1][4] = arg1;
    qDebug() << squares[1][4];
}
void Widget::on_checkBox_15_stateChanged(int arg1)
{
    squares[1][5] = arg1;
    qDebug() << squares[1][5];
}
void Widget::on_checkBox_16_stateChanged(int arg1)
{
    squares[1][6] = arg1;
    qDebug() << squares[1][6];
}

void Widget::on_checkBox_20_stateChanged(int arg1)
{
    squares[2][0] = arg1;
    qDebug() << squares[2][0];
}
void Widget::on_checkBox_21_stateChanged(int arg1)
{
    squares[2][1] = arg1;
    qDebug() << squares[2][1];
}
void Widget::on_checkBox_22_stateChanged(int arg1)
{
    squares[2][2] = arg1;
    qDebug() << squares[2][2];
}
void Widget::on_checkBox_23_stateChanged(int arg1)
{
    squares[2][3] = arg1;
    qDebug() << squares[2][3];
}
void Widget::on_checkBox_24_stateChanged(int arg1)
{
    squares[2][4] = arg1;
    qDebug() << squares[2][4];
}
void Widget::on_checkBox_25_stateChanged(int arg1)
{
    squares[2][5] = arg1;
    qDebug() << squares[2][5];
}
void Widget::on_checkBox_26_stateChanged(int arg1)
{
    squares[2][6] = arg1;
    qDebug() << squares[2][6];
}

void Widget::on_checkBox_30_stateChanged(int arg1)
{
    squares[3][0] = arg1;
    qDebug() << squares[3][0];
}
void Widget::on_checkBox_31_stateChanged(int arg1)
{
    squares[3][1] = arg1;
    qDebug() << squares[3][1];
}
void Widget::on_checkBox_32_stateChanged(int arg1)
{
    squares[3][2] = arg1;
    qDebug() << squares[3][2];
}

void Widget::on_checkBox_34_stateChanged(int arg1)
{
    squares[3][4] = arg1;
    qDebug() << squares[3][4];
}
void Widget::on_checkBox_35_stateChanged(int arg1)
{
    squares[3][5] = arg1;
    qDebug() << squares[3][5];
}
void Widget::on_checkBox_36_stateChanged(int arg1)
{
    squares[3][6] = arg1;
    qDebug() << squares[3][6];
}

void Widget::on_checkBox_40_stateChanged(int arg1)
{
    squares[4][0] = arg1;
    qDebug() << squares[4][0];
}
void Widget::on_checkBox_41_stateChanged(int arg1)
{
    squares[4][1] = arg1;
    qDebug() << squares[4][1];
}
void Widget::on_checkBox_42_stateChanged(int arg1)
{
    squares[4][2] = arg1;
    qDebug() << squares[4][2];
}
void Widget::on_checkBox_43_stateChanged(int arg1)
{
    squares[4][3] = arg1;
    qDebug() << squares[4][3];
}
void Widget::on_checkBox_44_stateChanged(int arg1)
{
    squares[4][4] = arg1;
    qDebug() << squares[4][4];
}
void Widget::on_checkBox_45_stateChanged(int arg1)
{
    squares[4][5] = arg1;
    qDebug() << squares[4][5];
}
void Widget::on_checkBox_46_stateChanged(int arg1)
{
    squares[4][6] = arg1;
    qDebug() << squares[4][6];
}

void Widget::on_checkBox_50_stateChanged(int arg1)
{
    squares[5][0] = arg1;
    qDebug() << squares[5][0];
}
void Widget::on_checkBox_51_stateChanged(int arg1)
{
    squares[5][1] = arg1;
    qDebug() << squares[5][1];
}
void Widget::on_checkBox_52_stateChanged(int arg1)
{
    squares[5][2] = arg1;
    qDebug() << squares[5][2];
}
void Widget::on_checkBox_53_stateChanged(int arg1)
{
    squares[5][3] = arg1;
    qDebug() << squares[5][3];
}
void Widget::on_checkBox_54_stateChanged(int arg1)
{
    squares[5][4] = arg1;
    qDebug() << squares[5][4];
}
void Widget::on_checkBox_55_stateChanged(int arg1)
{
    squares[5][5] = arg1;
    qDebug() << squares[5][5];
}
void Widget::on_checkBox_56_stateChanged(int arg1)
{
    squares[5][6] = arg1;
    qDebug() << squares[5][6];
}

void Widget::on_checkBox_60_stateChanged(int arg1)
{
    squares[6][0] = arg1;
    qDebug() << squares[6][0];
}
void Widget::on_checkBox_61_stateChanged(int arg1)
{
    squares[6][1] = arg1;
    qDebug() << squares[6][1];
}
void Widget::on_checkBox_62_stateChanged(int arg1)
{
    squares[6][2] = arg1;
    qDebug() << squares[6][2];
}
void Widget::on_checkBox_63_stateChanged(int arg1)
{
    squares[6][3] = arg1;
    qDebug() << squares[6][3];
}
void Widget::on_checkBox_64_stateChanged(int arg1)
{
    squares[6][4] = arg1;
    qDebug() << squares[6][4];
}
void Widget::on_checkBox_65_stateChanged(int arg1)
{
    squares[6][5] = arg1;
    qDebug() << squares[6][5];
}
void Widget::on_checkBox_66_stateChanged(int arg1)
{
    squares[6][6] = arg1;
    qDebug() << squares[6][6];
}

void Widget::on_spinBox_3_valueChanged(int arg1)
{
    threshold = arg1;
}

void Widget::on_comboBox_2_currentIndexChanged(int index)
{
    automataTypeTwo = index;
}
