#include "board.h"

#include <QDebug>
#include <QStringList>
#include "math.h"
#include <QDateTime>
#include <rollitia.h>

board::board()
{
    for (int i=0;i<8;i++) {

        for (int j=0;j<8;j++) {
        board_tab[i][j]=0;
        board_tab_free[i][j]=1;
        }
    }

QString str=place(3,3);
str.append(place(3,4));
str.append(place(4,3));
str.append(place(4,4));
setFreePos(str);

//ia=new RollitIA();


}

QString board::place(int x,int y)
{

QString res;
QString astr,bstr;

int a,b;
for (a=x-1;a<x+2;a++)
{ for (b=y-1;b<y+2;b++)
    {

        if ((a>-1)&&(a<8)&&(b>-1)&&(b<8))
        {
            astr.setNum(a,10);
            bstr.setNum(b,10);


            res.append(astr+"/"+bstr+"|");


                }

    }
}

return res;

}

void board::setFreePos(QString str)
{
    QString str2;
    QStringList s=str.split("|",QString::SkipEmptyParts);
    for (int i=0;i<s.size();i++)
    {


        str2=s[i];
        QStringList s2=str2.split("/",QString::SkipEmptyParts);
        QString xstr=s2[0];
        QString ystr=s2[1];

        bool ok;
        int x=xstr.toInt(&ok,10);
        int y=ystr.toInt(&ok,10);

        board_tab_free[x][y]=0;


    }



}


int board::getValue(QString str)
{

    QStringList spl=str.split("/");

    QString xstr=spl[0];
    QString ystr=spl[1];

    bool ok;
    int x=xstr.toInt(&ok,10);
    int y=ystr.toInt(&ok,10);

    return board_tab[x][y];

}

int board::getValue(int x,int y)
{

    return board_tab[x][y];
}

void board::invert(QString str)
{

    QStringList spl=str.split("/");

    QString xstr=spl[0];
    QString ystr=spl[1];

    bool ok;
    int x=xstr.toInt(&ok,10);
    int y=ystr.toInt(&ok,10);

    if (board_tab[x][y]==1) board_tab[x][y]=2;
    if (board_tab[x][y]==2) board_tab[x][y]=1;

    return;
}

void board::playIA(void)
{


return;

}

QString parse(QString combinaison)
{



int pos0=combinaison.indexOf("52");
int pos1=combinaison.indexOf("21");

if (pos1>pos0)
{
if (combinaison.contains("521")) combinaison.replace("521","511");
if (combinaison.contains("5221")) combinaison.replace("5221","5111");
if (combinaison.contains("52221")) combinaison.replace("52221","51111");
if (combinaison.contains("522221")) combinaison.replace("522221","511111");


}



{
int pos0=combinaison.indexOf("12");
int pos1=combinaison.indexOf("25");

if (pos1>pos0)
{
if (combinaison.contains("125")) combinaison.replace("125","115");
if (combinaison.contains("1225")) combinaison.replace("1225","1115");
if (combinaison.contains("12225")) combinaison.replace("12225","11115");
if (combinaison.contains("122225")) combinaison.replace("122225","111115");

//return combinaison;
}




}

return combinaison;

}

void board::ProcessHV(int x,int y)
{

    //int numH=0;

    QVector<int> v1;
    QVector<int> v2;

    QString comb,toStr,combA;
    QString comb2,toStr2;


    int k=0;

        k=0;
            for (int j=0;j<8;j++)
                {
                v2.append(board_tab[x][j]);
                toStr.setNum(board_tab[x][j]);
                comb2.append(toStr);

                 }
            comb2[y]='5';

            for (int j=0;j<8;j++)
                {
                v1.append(board_tab[j][y]);
                toStr.setNum(board_tab[j][y]);
                comb.append(toStr);

                 }

            comb[x]='5';

            qDebug() << "combH: " << comb << endl;
            qDebug() << "combV: " << comb2 << endl;


        QString sol=parse(comb);


        for (int j=0;j<8;j++)
            {
            if (sol[j]=='1') board_tab[j][y]=1;
            if (sol[j]=='5') board_tab[j][y]=1;
            if (sol[j]=='2') board_tab[j][y]=2;

            }

        qDebug() << "sol: " << sol << endl;

        sol=parse(comb2);


        for (int j=0;j<8;j++)
            {

            if (sol[j]=='1') board_tab[x][j]=1;
            if (sol[j]=='5') board_tab[x][j]=1;
            if (sol[j]=='2') board_tab[x][j]=2;

            }

        qDebug() << "sol: " << sol << endl;

        return;

}

void board::processDiag(int x,int y)
{


QStringList diagW;
diagW << "0/0|1/1|2/2|3/3|4/4|5/5|6/6|7/7|";
diagW << "2/0|3/1|4/2|5/3|6/4|7/5|";
diagW << "4/0|5/1|6/2|7/3|";
diagW << "6/0|7/1|";
diagW << "0/2|1/3|2/4|3/5|4/6|5/7|";
diagW << "0/4|1/5|2/6|3/7|";
diagW << "0/6|1/7|";
diagW << "2/0|1/1|0/2|";
diagW << "4/0|3/1|2/2|1/3|0/4|";
diagW << "6/0|5/1|4/2|3/3|2/4|1/5|0/6|";
diagW << "7/1|6/2|5/3|4/4|3/5|2/6|1/7|";
diagW << "7/3|6/4|5/5|4/6|3/7|";
diagW << "7/5|6/6|5/7|";

QStringList diagB;
diagB << "7/0|6/1|5/2|4/3|3/4|2/5|1/6|0/7|";
diagB << "5/0|4/1|3/2|2/3|1/4|0/5|";
diagB << "3/0|2/1|1/2|0/3|";
diagB << "1/0|0/1|";
diagB << "7/2|6/3|5/4|4/5|3/6|2/7|"; // =9
diagB << "7/4|6/5|5/6|4/7|";  // =11
diagB << "7/6|6/7|";         // =13
diagB << "0/1|1/2|2/3|3/4|4/5|5/6|6/7|"; // =-1
diagB << "1/0|2/1|3/2|4/3|5/4|6/5|7/6|"; // =1
diagB << "3/0|4/1|5/2|6/3|7/4|"; // =3
diagB << "5/0|6/1|7/2|"; // =5
diagB << "0/3|1/4|2/5|3/6|4/7|"; // =-3
diagB << "0/5|1/6|2/7|"; // =-5

QVector<int> dat3;

if ((x+y)%2!=0) {

    QStringList test2;

    QString strr,strrx,strry;

    strr.append(strrx.setNum(x,10));
    strr.append("/");
    strr.append(strry.setNum(y,10));

    qDebug() << strr;

    for (int i=0;i<diagB.size();i++)
    {

        QString test=diagB[i];

        if(test.contains(strr)) test2.append(test);

    }

   for (int j=0;j<test2.size();j++)
    {


   QString dat=test2[j];

   qDebug() << dat;

   QStringList dat2=dat.split("|",QString::SkipEmptyParts);
   QString dat4="";
   for (int k=0;k<dat2.size();k++)
   {
       int va=getValue(dat2[k]);
       //qDebug() << va;

       QString t;
       t.setNum(va,10);
       dat4=dat4+t;

   }

   qDebug() << "dat4:" << dat4 ;

   if (dat4.contains("121")) {

   int pos=dat4.indexOf("121",0);
   if (pos!=-1)
   {
   invert(dat2[pos+1]);
   }

   }

   if (dat4.contains("1221")) {



       int pos=dat4.indexOf("1221",0);
       if (pos!=-1)
       {
       invert(dat2[pos+1]);
       invert(dat2[pos+2]);
       }

    }


}

}



}

