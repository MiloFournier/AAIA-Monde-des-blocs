//test?
#include <queue>
#include <unordered_map>
#include <limits>

using namespace std;

#include "state.cpp"
#include "stateGraph.cpp"

struct Info{
    int g;
    State pred;
    bool isGrey;
};

struct compare{
    // Used to compare elements in the priority queue
    bool operator()(pair<State,int> const& s1, pair<State,int> const& s2){
        return s1.second > s2.second;
    }
};

using PriorityQueue = priority_queue< pair<State,int>, vector<pair<State,int>>, compare >;
using HashMap = unordered_map<State, Info, State::hash>;

void printSolution(StateGraph &g, const State &s0, const State &s, const HashMap &map){
    // Print the sequence of actions to go from s0 to s
    if (!(s == s0)){
        printSolution(g, s0, map.at(s).pred, map);
        g.print(map.at(s).pred, s);
    }
}

void astar(const State &s0, StateGraph &g, int w){
    // Search for the shortest path to go from s0 to a final state in g
    clock_t start = clock(); // start time used to compute CPU time
    int nbIter = 0;
    HashMap map;
    PriorityQueue q;
    map[s0].g = 0;
    map[s0].isGrey = true;
    q.push({s0, g.heuristic(s0)});
    double borne = std::numeric_limits<double>::infinity();
    while (!q.empty()){
        State s = q.top().first;
        q.pop();
        if (!map[s].isGrey) continue; // s is ignored because it is black
        nbIter++;
        
        int nbActions = g.searchActions(s);
        for (int i=0; i<nbActions; i++){
            State ss = g.transition(s, i);
            if (map.count(ss)==0 || map[s].g + g.getCost(s, i) < map[ss].g){
                map[ss].isGrey = true;
                map[ss].g = map[s].g + g.getCost(s, i);
                map[ss].pred = s;
                if (g.isFinal(ss)){
                    borne = map.at(ss).g;
                    printf("Solution of length %d found in %d iterations and %f seconds\n", map.at(ss).g, nbIter, ((double) (clock() - start)) / CLOCKS_PER_SEC);
                }else if((map.at(ss).g + g.heuristic(ss) )< borne){
                    q.push( {ss, map[ss].g + w*g.heuristic(ss)} );
                }
                
            }
        }
        map[s].isGrey = false;
    }
    if(borne == std::numeric_limits<double>::infinity()){
        printf("The problem has no solution (number of iterations = %d; CPU time = %fs)\n", nbIter, (double) (clock() - start) / CLOCKS_PER_SEC);
    }else{
        printf("Optimal Solution of length %f found in %d iterations and %f seconds\n", borne, nbIter, ((double) (clock() - start)) / CLOCKS_PER_SEC);
    }
    
}

int main(){
    StateGraph g;
    astar(g.initialState(), g, 2);
    return 0;
}




