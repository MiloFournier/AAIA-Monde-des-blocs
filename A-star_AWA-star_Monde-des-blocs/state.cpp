/*
 Implementation of class State for the bloc world problem
 Copyright (C) 2023 Christine Solnon
 Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails.
 */

#include <iostream>

class State{
public:
    State(const State& s, int j, int k);
    // Precondition: 0<=j<stack.size(), 0<=k<stack.size(), and stack[i].length()>0
    // Constructor that creates the state obtained from s by moving the bloc
    // on the top of stack j to the top of stack k
    
    State(int nbStacks, int nbBlocs);
    // Return the state where nbBlocs blocs are evenly distributed on nbStacks stacks
    
    State();
    
    int getNbBlocs(int i) const;
    // Precondition: 0<=i<stack.size()
    // Return the number of blocs on stack i

    int getBloc(int i, int j) const;
    // Precondition: 0<=i<stack.size() and 0<=j<getNbBlocs(i)
    // Return the jth bloc on stack i

    void print() const;
    // Print this

    struct hash;
    // Hash function used by HashMap
    
    bool operator==(const State& s) const;
    // Return true if this=s
  
private:
    vector<string> stack;
    // For each i in [0..stack.size()-1], and each j in [0,stack[i].length()-1],
    // stack[i][j] = jth bloc on ith stack (from bottom to top)
};

State::State(int nbStacks, int nbBlocs){
    // Return the state where nbBlocs blocs are evenly distributed on nbStacks stacks
    stack = vector<string>(nbStacks, "");
    for (int j=0; j<nbBlocs; j++) stack[j%nbStacks] += 'a'+j;
}

State::State(const State& s, int j, int k){
    // Precondition: 0<=j<stack.size(), 0<=k<stack.size(), and stack[i].length()>0
    // Constructor that creates the state obtained from s by moving the bloc
    // on the top of stack j to the top of stack k
    int nbStacks = s.stack.size();
    stack = vector<string>(nbStacks);
    for (int i=0; i<nbStacks; i++){
        if (i == j) stack[i] = s.stack[i].substr(0, s.stack[i].length()-1);
        else if (i == k) stack[i] = s.stack[i]+s.stack[j].substr(s.stack[j].length()-1, 1);
        else stack[i] = s.stack[i];
    }
}

State::State(){};

int State::getNbBlocs(int i) const{
    // Precondition: 0<=i<nbStacks
    // Return the numbr of blocs on stack i
    return stack[i].length();
}

int State::getBloc(int i, int j) const{
    // Precondition: 0<=i<stack.size() and 0<=j<getNbBlocs(i)
    // Return the jth bloc on stack i
    return stack[i][j];
}

void State::print() const{
    // Print this
    for (uint k=0; k<stack.size(); k++)
        cout << "stack[" << k << "]=" << stack[k] << " ";
    printf("\n");
}

struct State::hash {
    // Hash function used by HashMap
    std::size_t operator()(const State& s) const{
        static string st;
        st = string(s.stack[0]);
        for (uint i=1; i<s.stack.size(); i++) st += " "+s.stack[i];
        return std::hash<string>{}(st);
    }
};

bool State::operator==(const State& s) const{
    // Return true if this=s
    for (uint i=0; i<stack.size(); i++)
        if (stack[i]!=s.stack[i]) return false;
    return true;
}
