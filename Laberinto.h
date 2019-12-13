#ifndef LABERINTO_H
#define LABERINTO_H

#include<vector>
#include<iostream>
#include"DisjSets.h"
#include<string>
#include <stdio.h>
#include <stdlib.h> 
#include <queue>
#include <stack>
#include <time.h> 
//working on a n*n space
enum PATH{BOT,RIGHT,TOP,LEFT};

class Laberinto{
private:
    struct WallHorizontal{
        int top,bot;
        bool up;
    };
    struct WallVertical{
        int left,right;
        bool up;
    };
    int filas, columnas;
    std::stack<int> pathie;
    DisjSets ds;

public:
    Laberinto(int fil, int colum):filas(fil),columnas(colum),ds(fil*colum){}
    ~Laberinto();
    void init();
    void print();
    int getAvailablePaths(int cell,std::vector<PATH> &v);
    int finalPath(int cell,std::vector<PATH> &v);
    int getWall(int cell,PATH p);
    void generateMaze();
    void generar();
    int findUsableCell(int cell,std::vector<PATH>&p);
    std::vector<Laberinto::WallHorizontal*> wh;
    std::vector<Laberinto::WallVertical*> wv;
};


void Laberinto::generateMaze(){ 
    int set;
    PATH currentPath;
    int cell;
    std::vector<PATH> p;
    for(int i=0; i<filas*columnas;++i){
        cell = i;
        if(getAvailablePaths(cell,p)>0){
            int random = rand()%(p.size());
            currentPath = p[random];
            int wall = getWall(cell,currentPath);//el path usando un wall
            if(currentPath==TOP ||currentPath==BOT){//wall horizontal
                if(wh[wall]->up){//SI HAY WALL damos al set2 el valor correspondiente
                    if(currentPath==TOP)
                        set = wh[wall]->top;
                    else if(currentPath==BOT)
                        set = wh[wall]->bot;
                }
            }
            else if(currentPath==LEFT || currentPath==RIGHT){//wall vertical
                if(wv[wall]->up){
                    if(currentPath==LEFT)
                        set = wv[wall]->left;
                    else if(currentPath==RIGHT)
                        set = wv[wall]->right;
                }            
            }
            ds.unionSets(ds.find(cell),ds.find(set));
            if(currentPath==LEFT || currentPath==RIGHT)
                wv[wall]->up = false;       
            if(currentPath==TOP || currentPath==BOT)
                wh[wall]->up = false;                     
        }
    }  
}
void Laberinto::generar(){
    int aleatoria;
    int columnasv = columnas -1;
    int filasv = filas;
    int columnash = columnas;
    int filash = filas-1;
    int cell = 0;
    bool done = false;
    std::vector<PATH> paths;
    pathie.push(cell);
    while(!done){
        if(getAvailablePaths(cell,paths)>0){
            cell = finalPath(cell,paths);
            if(cell == (filas*columnas) - 1)
                done = true;
        }
        print();
    }
}
int Laberinto::finalPath(int cell,std::vector<PATH> &p){
    bool done = false;
    int set;
    PATH currentPath;  
    while(!done){
        if(p.size()){
            int random = rand()%(p.size());
            currentPath = p[random];
            p.erase(p.begin() + random);
            int wall = getWall(cell,currentPath);//el path usando un wall
            if(currentPath==TOP ||currentPath==BOT){//wall horizontal
                if(wh[wall]->up){//SI HAY WALL damos al set2 el valor correspondiente
                    if(currentPath==TOP)
                        set = wh[wall]->top;
                    else if(currentPath==BOT)
                        set = wh[wall]->bot;
                    done = true;
                }
            }
            else if(currentPath==LEFT || currentPath==RIGHT){//wall vertical
                if(wv[wall]->up){
                    if(currentPath==LEFT)
                        set = wv[wall]->left;
                    else if(currentPath==RIGHT)
                        set = wv[wall]->right;
                    done = true;
                }            
            }
            if(done){
                if(ds.find(cell)==ds.find(set)){
                    done = false;
                }
                else{
                    ds.unionSets(ds.find(cell),ds.find(set));
                    if(currentPath==LEFT || currentPath==RIGHT)
                        wv[wall]->up = false;       
                    if(currentPath==TOP || currentPath==BOT)
                        wh[wall]->up = false;
                }
            }
        }
        else{
            set = findUsableCell(cell,p);
        }
    }
    pathie.push(set);
    return set;
}
int Laberinto::findUsableCell(int cell,std::vector<PATH>&p){
    bool done = false;
    int prevcell ;
    while(!done){
        prevcell = pathie.top();
        if(getAvailablePaths(prevcell,p)>0)
            done = true;
        else
            pathie.pop();
    }
    return prevcell;
}
int Laberinto::getAvailablePaths(int cell, std::vector<PATH> &v){
    //arreglar para una matriz n*m just working n * n
    v.clear();
    if(!(cell%filas) && cell && cell != filas*(filas-1)){//multiplo de n y diferente de 0 y diferente de la ultima casilla
        v.push_back(BOT);
        v.push_back(RIGHT);
        v.push_back(TOP);// T,B,R
    }
    else if(!cell){//si es 0
        v.push_back(BOT);
        v.push_back(RIGHT);// B,R
    }
    else if(cell == filas*(filas-1)){
        v.push_back(RIGHT);
        v.push_back(TOP); //T,R
    }
    else if( !((cell+1)%filas) && cell!= filas-1 && cell != (filas*filas)-1){
        v.push_back(BOT);
        v.push_back(TOP);
        v.push_back(LEFT); // L,T,B
    }
    else if(cell == filas-1){
        v.push_back(BOT);
        v.push_back(LEFT); //L,B
    }
    else if(cell == (filas*filas)-1){
        v.push_back(TOP);
        v.push_back(LEFT);//T,L
    }
    else if(cell > 0 && cell < filas-1){
        v.push_back(BOT);
        v.push_back(RIGHT);
        v.push_back(LEFT); //L,B,R
    }
    else if(cell > filas*(filas-1) && cell < (filas*filas)-1){
        v.push_back(RIGHT);
        v.push_back(TOP);
        v.push_back(LEFT);//L,T,R
    }
    else{
        v.push_back(BOT);
        v.push_back(RIGHT);
        v.push_back(TOP);
        v.push_back(LEFT);//L,T,R,B
    }
    int i=0;
    while(v.size() && i < v.size()){//returning realavailablepaths
        int walltemp = getWall(cell,v[i]);
        if(v[i]==TOP){
            if(wh[walltemp]->up)
                if(ds.find(cell) != ds.find(wh[walltemp]->top))//comparamos si no estan en el mismo conjunto
                    i++;                                        //es un realpath available
                else
                    v.erase(v.begin()+i);                       //si estan en el mismo conjunto lo borramos
            else
                v.erase(v.begin()+i);                           //si el wall ya esta siendo usado
        }
        else if(v[i]==BOT){
            if(wh[walltemp]->up)
                if(ds.find(cell)!= ds.find(wh[walltemp]->bot))
                    i++;
                else
                    v.erase(v.begin()+i);
            else
                v.erase(v.begin()+i);
        }
        else if(v[i]==RIGHT){
            if(wv[walltemp]->up)
                if(ds.find(cell)!= ds.find(wv[walltemp]->right))
                    i++;
                else
                    v.erase(v.begin()+i);
            else
                v.erase(v.begin()+i);
        }
        else if(v[i]==LEFT){
            if(wv[walltemp]->up)
                if(ds.find(cell)!= ds.find(wv[walltemp]->left))
                    i++;
                else
                    v.erase(v.begin()+i);
            else
                v.erase(v.begin()+i);
        }
    }
    return v.size();
}
int Laberinto::getWall(int cell,PATH p){
    if(p==RIGHT || p==LEFT){
        if(p==RIGHT){
            return cell-(cell/filas);
        }
        else if(p==LEFT)
            return cell-((cell/filas)+1);
    }
    else if(p==TOP || p==BOT){
        if(p==TOP){
            return cell - filas;
        }
        else if(p==BOT)
            return cell;
    }
}
void Laberinto::print(){
    
    std::string estado;
    std::cout<<"HORIZONTAL"<<std::endl;
    for(int i=0;i < wh.size();++i){
        if(wh[i]->up)
            estado = "up";
        else
            estado = "down";        
        std::cout<<i <<" top: "<< wh[i]->top<<" bot: "<< wh[i]->bot<<" estado: "<<estado<<std::endl;
    }
    std::cout<<"VERTICAL"<<std::endl;
    for(int i=0;i < wv.size();++i){
        if(wv[i]->up)
            estado = "up";
        else
            estado = "down";
        std::cout<<i <<" left: "<< wv[i]->left<<" right: "<< wv[i]->right<<" estado: "<<estado<<std::endl;
    }
    for( int i = 0; i < filas*columnas; ++i ){
        std::cout << ds.find( i ) << "*";
        
    }
}
void Laberinto::init(){
    int columnasv = columnas -1;
    int filasv = filas;
    int columnash = columnas;
    int filash = filas-1;

    WallHorizontal* newH;
    WallVertical* newV;
    //init vertical
    for (int i = 0; i < filasv; ++i){
        for(int j= 0; j < columnasv;++j){
            newV = new WallVertical;
            newV->left = (columnasv+1)*i+j;
            newV->right = (columnasv+1)*i+j+1;
            newV->up = true;
            wv.push_back(newV);
        }
    }
    for (int i = 0; i < filash; ++i){
        for(int j= 0; j < columnash;++j){
            newH = new WallHorizontal;
            newH->bot = columnash*i+j+columnash;
            newH->top = columnash*i+j;
            newH->up = true;
            wh.push_back(newH);
        }
    }
}

Laberinto::~Laberinto(){
    std::cout<<"HORIZONTAL"<<std::endl;
    for(int i=0;i < wh.size();++i){
        delete wh[i];
    }
    std::cout<<"VERTICAL"<<std::endl;
    for(int i=0;i < wv.size();++i){
        delete wv[i];
    }
}




#endif