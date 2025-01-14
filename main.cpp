#include<iostream>
#include<vector>
#include "raylib.h"
#include "perc.cpp"
#include "point.cpp"


void drawWindow(int height, int width, vector<point> &pt, vector<point> &ptTest);
int drawPt(vector<point> &pt, int i, int error);

void init();

using namespace std;

perceptron perc; //initializing the perceptron only once

const int screenWidth = 600;
const int screenHeight = 600;
int numPoints = 1000;
int frameRate = 150;
int maxPasses = 5;

int main()
{   
    init();

    vector<point> pt;
    for (int i = 0; i < numPoints; i++)
    {   
        point newpt(screenHeight,screenWidth);
        pt.push_back(newpt);
    }   

    vector<point> ptTest;
        for (int i = 0; i < numPoints; i++)
        {   
            point newpt(screenHeight,screenWidth);
            ptTest.push_back(newpt);
        } 
    
    drawWindow(screenHeight,screenWidth, pt, ptTest);

    CloseWindow(); 
    
    return 0;
}

/*
    Draws the window and acts as the main loop for triggering the core functions
*/
void drawWindow(int height, int width, vector<point> &pt, vector<point> &ptTest){
    
    ClearBackground(BLACK);
    DrawText("ACC: ",10,630,40,DARKGRAY);
    DrawText("N/A",120,630,40,DARKGRAY);

    DrawText("PASS: ",370,630,40,DARKGRAY);
    DrawText("0",510,630,40,DARKGRAY);

    

    int i = 0;
    int error = 0;
    float total_error = 0;
    int pass = 0;
    int g_acc = 0;

    for (;!WindowShouldClose(); i++)
    {   
        BeginDrawing();

            //training portion

            vector<float> inputs = {pt[i].x,pt[i].y};
            int target = pt[i].label;
                
            if(pass>0)perc.train(inputs,target); //training part

            if(pass<maxPasses){
                DrawText("[TRAINING]",170,680,40,BLUE);
                DrawLine(0,0,screenHeight,screenWidth,LIGHTGRAY);

                total_error += drawPt(pt,i,error); //calling the draw function

                if(i==pt.size()-1){
                    i=0;
                    error = 0;
                    pass++;
                    string pass_str = to_string(pass);
                    float acc_fl = (float)(100-(total_error/(float)numPoints)*100);
                    g_acc = acc_fl;
                    string accuracy = to_string(acc_fl)+"%";
                    cout<<"Accuracy: "<<accuracy<<endl;
                    DrawRectangle(110,620,240,50,BLACK);
                    DrawRectangle(510,630,80,50,BLACK);
                    
                    if(acc_fl==100.000000){
                        DrawText("100%(FULL)",120,630,40,BLUE);
                    } else {
                        DrawText(accuracy.c_str(),120,630,40,BLUE);
                    }
                    cout<<"[weights] "<<perc.getWeights()[0]<<" "<<perc.getWeights()[1]<<endl;
                    DrawText(pass_str.c_str(),510,630,40,DARKGRAY);
                    total_error = 0;

                    
                }
                
                if(pass==maxPasses){   
                    ClearBackground(BLACK);
                    DrawRectangle(0,screenHeight+80,screenWidth,50,BLACK);
                    total_error = 0;
                    DrawText("[TESTING]",200,680,40,RED); 
                    DrawText("ACC: ",10,630,40,DARKGRAY);

                    DrawText("PASS: ",370,630,40,DARKGRAY);
                    DrawText("ACC: ",10,740,40,DARKGRAY);
                }
                
                
            }    

            //test portion

            else if(pass<maxPasses+1&&pass>maxPasses-1){

                error = 0;
                

                DrawLine(0,0,screenHeight,screenWidth,LIGHTGRAY);

                if(pass==maxPasses)total_error += drawPt(ptTest,i,error); //calling the draw function

                if(i==pt.size()-1){
                    float testAcc = (float)(100-(total_error/(float)numPoints)*100);

                    cout<<"Test Accuracy: "<<testAcc<<"%"<<endl;
                    if(testAcc==100.000000){
                        DrawText("100%(FULL) [TEST]",120,740,40,RED);
                    } else {
                        DrawText((to_string(testAcc)+"%"+" [TEST]").c_str(),120,740,40,RED);
                    }
                    pass++;
                }
            }

        EndDrawing();
    }

}

/*
    First Draws the points in the coordinates then passes the point coordinates as inputs
    and the points labels as the target values to the train function. 
*/
int drawPt(vector<point> &pt, int i, int error){

    if(pt[i].label==1){
        DrawCircle(pt[i].x,pt[i].y,6,GRAY);
        DrawCircle(pt[i].x,pt[i].y,4,RAYWHITE);
    } else {
        DrawCircle(pt[i].x,pt[i].y,6,DARKGRAY);
    }
    vector<float> inputs = {pt[i].x,pt[i].y};
    int target = pt[i].label;

    int eduGuess = perc.think(inputs); 

    if(eduGuess == target){
        DrawCircle(pt[i].x, pt[i].y,4,DARKGREEN);
    } else {
        DrawCircle(pt[i].x, pt[i].y,4,RED);
        error++;
        // perc.train(inputs,target);
    }

    return error;
    
}


/*
    Necessary Raylib initializations
*/
void init(){

    InitWindow(screenWidth, screenHeight+180, "Perceptron classification");
    SetTargetFPS(frameRate);

}