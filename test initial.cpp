#include <iostream>
#include <math.h>

using namespace std;

void citesteprajitura(float v[4]){

int forma;
cin>>forma;

v[4] = forma;

if(forma == 1){
    cin>>v[0];
    v[1] = 3.14*v[0];
}
else if(forma == 2){
    cin>>v[0]>>v[1];
}
else if(forma == 3){
    cin>>v[0];
    v[1] = v[0];
}
else{
    cin>>v[0]>>v[1];
    v[1] /= 2;
}

float h;
cin>>h;

v[2] = h;

}

void total(int v[4]){
    int volum = v[0] * v[1] * v[2];
    int al = 2 * v[0] * v[1];
    if(v[4] == 1){
        al += 2 * v[1] * v[2];
    }
    else if(v[4] == 2){
        al += 2 * (v[0] + v[1]) * v[2];
    }
    else if(v[4]) == 3{
        al += 4 * v[0] * v[2];
    }
    else{
        al += (v[0] + v[1] + sqrt(v[0]*v[0] + v[1]*v[1])) * v[2];
    }
    cout<<"volum: "<<volum;
    cout<<"arie laterala: "<<al;
}

int main()
{

    float p[4];
    citesteprajitura(p);
    cout<<p[0]<<endl<<p[1]<<endl<<p[2];
    return 0;

}
