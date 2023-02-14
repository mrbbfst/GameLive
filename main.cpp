#include <vector> 
#include <iostream> 
#include <stdlib.h> 
#include <ctime> 
#include <forward_list> 
#include <chrono>
#include <thread>
#include <algorithm>


#define SIZE_X 50 
#define SIZE_Y 30
#define LIVE true
#define DEAD false

#define LIVE_CH "\e[42m  "
#define DEAD_CH "\e[45m  "
#define ENDL "\e[0m\n"
#define CLEARCLS "\033c"

#define LIVE_WINDOW_HEIGHT 500
#define LIVE_WINDOW_WEIGHT 500


using Scene = std::vector< std::vector< bool > >; 


struct p { 
    public: 
        p(int y, int x, bool state) : y(y), x(x), state(state) {}; 
        int y; 
        int x; 
        bool state;

        void move(Scene& scene) {
            scene[y][x] = state;
            /*for(auto iter = pull.begin();iter!=pull.end();++iter)
                scene[(*iter).y][(*iter).x] = (*iter).state;*/
        }

        /*operator char*() const {
            return (state ? LIVE_CH : DEAD_CH );
            };*/
};
 
class Observe { 
    public: 
    void Compute(std::vector< std::vector<bool> >&scene)  { 
        for(int y=0; y<scene.size(); y++) 
            for(int x=0;x<scene[y].size();x++) 
                IsAliveR(scene, y,x); 
                std::for_each(pull.cbegin(),pull.cend(), [&scene](p n) {n.move(scene);});
                this->clearpool();
 
    }
 
    void seed(Scene& scene) { 
        std::srand(std::time(nullptr)); 
        for(int ity=0;ity<SIZE_Y;ity++) 
        for(int itx=0;itx<SIZE_X;itx++) 
            scene[ity][itx] = rand()%2; 
    }
 
 
    private:  
        void IsAliveR(Scene& scene, int y, int x) {
            char livingNeighbors = countLivingNeighbors(y,x,scene);

            if(scene[y][x] && (livingNeighbors <2 || livingNeighbors > 3))
                pull.push_front(p(y,x,DEAD));
            if((!(scene[y][x])) && livingNeighbors==3)
                pull.push_front(p(y,x,LIVE));

        }

        char countLivingNeighbors(int y, int x, Scene& scene) {
            char count = 0;
            count+=((y>0 && x>0) && scene[y-1][x-1]);    //left-up
            count+=(y && scene[y-1][x]);    ; //up  
            count+=((x!= SIZE_X-1 && y>0) && scene[y-1][x+1]);  //right-up
            count+=(x!= SIZE_X-1 && scene[y][x+1]); //right
            count+=(x< SIZE_X-1 && y<SIZE_Y-1 && scene[y+1][x+1]); //right-down
            count+=(y!=SIZE_Y-1 && scene[y+1][x]); //down
            count+=(x && y!=SIZE_Y-1 && scene[y+1][x-1]); //left-down    
            count+=(x && scene[y][x-1]);//left
            return count;
        }

        void clearpool() {
            pull.clear();
        }

        bool searchonpull(int y, int x) { 
            bool flag=false; 
            auto iter=pull.begin(); 
            while(!flag || (++iter)!=pull.end()) 
            flag = ((*iter).y==y && (*iter).x==x); 
            return flag; 
         }
          
        
        std::forward_list<p> pull; 
}; 

 
void clear() { 
    std::cout << CLEARCLS;
} 
 /*
template<typename T, typename S>
bool in(T& ar, S s) {
    bool flag=false; 
    auto iter=ar.cbegin(); 
    while(!flag || (++iter)!=ar.cend()) 
        flag = (*iter) == s; 
    return flag; 

}*/

void visualize(Scene& scene) {
    for(int y = 0;y<SIZE_Y;y++) 
            for(int x =0; x<SIZE_X;x++) { 
                std::cout << (scene[y][x] ? LIVE_CH : DEAD_CH ); 
                if(x==SIZE_X-1) 
                    std::cout<<ENDL; 
            }
}


int main(int argc, char* argv[]) { 
    Scene scene(SIZE_Y, std::vector<bool>(SIZE_X)); 
    Observe ob; 
    
    ob.seed(scene); 
    visualize(scene);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    char c = 0; 
    while(true && c!='q') { 
        clear(); 
        ob.Compute(scene); 
        visualize(scene);
             
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
      
        } 
 
    return 0; 
}