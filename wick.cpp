#include <bits/stdc++.h>

using namespace std;

struct half_edge { int id; int type; bool paired; };

typedef vector<half_edge> vertex;
typedef tuple<half_edge,half_edge> edge;
struct state { vector<vertex> vertices; vector<edge> edges; map<int,int> m; };

bool contract_compatible(const int& type1, const vector<tuple<int,int>>& allowed_type_pairs)
{
    bool found = false;
    for(auto& p : allowed_type_pairs)
        found = found || type1 == get<0>(p);
    return found;
}
bool contract_compatible(const int& type1, const int& type2, const vector<tuple<int,int>>& allowed_type_pairs)
{
    bool found = false;
    for(auto& p : allowed_type_pairs)
        found = found || tie(type1,type2) == p;
    return found;
}
bool contract_compatible(const half_edge& particle1, const vector<tuple<int,int>>& allowed_type_pairs){
    return contract_compatible(particle1.type, allowed_type_pairs);
}
bool contract_compatible(const half_edge& particle1, const half_edge& particle2, const vector<tuple<int,int>>& allowed_type_pairs){
    return contract_compatible(particle1.type,particle2.type, allowed_type_pairs);
}
map<int,int> gen_map(const vector<vertex>& vertices){
    map<int,int> m;
    for(int i = 0; i < vertices.size(); ++i)
        for(int j = 0; j < vertices[i].size(); ++j)
            m[vertices[i][j].id] = i;
    return m;
}



bool operator<(const half_edge& lhs, const half_edge& rhs) { 
    return lhs.id < rhs.id;
}
bool operator>(const half_edge& lhs, const half_edge& rhs) { 
    return rhs.id < lhs.id;
}
bool operator==(const half_edge& lhs, const half_edge& rhs) { 
    return lhs.id == rhs.id;
}
bool operator!=(const half_edge& lhs, const half_edge& rhs) { 
    return lhs.id == rhs.id;
}
bool operator>=(const half_edge& lhs, const half_edge& rhs) { 
    return lhs.id >= rhs.id;
}
bool operator<=(const half_edge& lhs, const half_edge& rhs) { 
    return lhs.id <= rhs.id;
}

bool operator<(const state& lhs, const state& rhs) { 
    assert(lhs.vertices.size() == rhs.vertices.size());
    assert(lhs.edges.size()    == rhs.edges.size());
    
    auto les = lhs.edges;
    auto res = rhs.edges;

    sort(les.begin(), les.end());
    sort(res.begin(), res.end());

    for(int i = 0; i < les.size(); ++i)
        if(les[i] >= res[i])
            return false;
    return true;
};
bool operator==(const state& lhs, const state& rhs) { 
    assert(lhs.vertices.size() == rhs.vertices.size());
    assert(lhs.edges.size()    == rhs.edges.size());
    
    auto les = lhs.edges;
    auto res = rhs.edges;

    sort(les.begin(), les.end());
    sort(res.begin(), res.end());

    for(int i = 0; i < les.size(); ++i)
        if(les[i] != res[i])
            return false;
    return true;
};

std::ostream& operator<<(std::ostream& os, const half_edge& he)
{
    os << "{" << he.id << " " << he.type << " " << (he.paired ? "pr" : "unpr") << "}";
    return os;
}
std::ostream& operator<<(std::ostream& os, const vector<half_edge>& vhe)
{
    os << "{  ";
    for(auto& he : vhe)
        os << he << ",  ";
    os << "}";
    return os;
}
std::ostream& operator<<(std::ostream& os, const edge& e)
{
    os << "{  " << get<0>(e) << ",  " << get<1>(e) << "  }";
    return os;
}
std::ostream& operator<<(std::ostream& os, const vector<vertex>& vertices)
{
    os << "{" << endl;
    for(auto& v : vertices)
        os << "     " << v << "," << endl;
    os << "}";
    return os;
}
std::ostream& operator<<(std::ostream& os, const vector<edge>& es)
{
    os << "{" << endl;
    for(auto& e : es)
        os << "     " << e << "," << endl;
    os << "}";
    return os;
}
std::ostream& operator<<(std::ostream& os, const state& s)
{
    os << "{ " << endl;
    os << "\"vertices\" : " << s.vertices << ", " << endl;
    os << "\"edges\"    : " << s.edges << endl;
    os << "}" << endl;
    return os;
}
std::ostream& operator<<(std::ostream& os, const vector<state>& vs)
{
    os << "vectorstate : { " << endl;
    for(auto& s : vs)
        os << s << ", " << endl;
    os << "}" << endl;
    return os;
}
vector<state> wickonceordered(const state& s, const vector<tuple<int,int>>& allowed_type_pairs)
{
    state modified_state;
    vector<state> gen_state;
    auto& v = s.vertices;
    auto& e = s.edges;

    int ind_vertex, ind_halfedge; 
    bool found_unpaired = false;

    for(int i = 0; i < v.size(); ++i){
        for(int j = 0; j < v[i].size() && !found_unpaired; ++j){
            if (v[i][j].paired == false && contract_compatible(v[i][j], allowed_type_pairs)){
                found_unpaired = true;
                tie(ind_vertex,ind_halfedge) = tie(i,j);
            }
        }
    }
    if(!found_unpaired)
        return gen_state;

    auto& vertex_init = v[ind_vertex][ind_halfedge];
    for(int i = ind_vertex+1; i < v.size(); ++i){
        for(int j = 0; j < v[i].size(); ++j){
            if(v[i][j].paired == false && contract_compatible(vertex_init, v[i][j], allowed_type_pairs)){
                modified_state = s;
                modified_state.vertices[ind_vertex][ind_halfedge].paired = true;
                modified_state.vertices[i][j].paired = true;
                modified_state.edges.push_back({vertex_init,v[i][j]});
                gen_state.push_back(modified_state);
            }
        }
    }
    return gen_state;
}
vector<state> wickonceordered(const vector<state>& vs, const vector<tuple<int,int>>& allowed_type_pairs)
{
    vector<state> configs, tmp_config;
    for(auto& s : vs){
        tmp_config = wickonceordered(s,allowed_type_pairs);
        configs.insert(configs.end(), tmp_config.begin(), tmp_config.end());
    }
    return configs;
}
vector<state> wickonceallpermutations(const state& s, const vector<tuple<int,int>>& allowed_type_pairs)
{
    state modified_state;
    vector<state> gen_state;
    auto& v = s.vertices;

    for(int i = 0; i < v.size(); ++i){
        for(int j = 0; j < v[i].size(); ++j){
            if (v[i][j].paired == false && contract_compatible(v[i][j], allowed_type_pairs)){

                for(int p = i+1; p < v.size(); ++p){
                    for(int q = 0; q < v[p].size(); ++q){
                        if(v[p][q].paired == false && contract_compatible(v[i][j], v[p][q], allowed_type_pairs)){

                            modified_state = s;
                            modified_state.vertices[i][j].paired = true;
                            modified_state.vertices[p][q].paired = true;
                            modified_state.edges.push_back({v[i][j],v[p][q]});
                            gen_state.push_back(modified_state);

                        }
                    }
                }


            }
        }
    }
    return gen_state;
}
vector<state> wickonceallpermutations(const vector<state>& vs, const vector<tuple<int,int>>& allowed_type_pairs)
{
    vector<state> configs, tmp_config;
    for(auto& s : vs){
        tmp_config = wickonceallpermutations(s,allowed_type_pairs);
        configs.insert(configs.end(), tmp_config.begin(), tmp_config.end());
    }
    return configs;
}

void solve1(){
    vertex vertex1 = { {0,0,0}, {1,0,0}, {2,0,0}, {3,0,0} };
    vertex vertex2 = { {4,0,0}, {5,0,0}, {6,0,0}, {7,0,0} };

    vector<state> config = {  {  {vertex1, vertex2}, vector<edge>(), gen_map({vertex1,vertex2})  } };
    vector<tuple<int,int>> all_allowed_pairings  = { {0,0}, {1,-1}, {-1,1} };
    vector<tuple<int,int>> fermion_pairings_only = { {1,-1}, {-1,1} };
    vector<tuple<int,int>> boson_pairings_only = { {0,0} };

    // cout << "----------- Wick 0 -----------" << endl;
    // cout << config[0] << endl;
    // cout << "----------- Wick 1 -----------" << endl;
    // config = wickonce(config, all_allowed_pairings);
    // cout << config << endl;
    // cout << "----------- Wick 2 -----------" << endl;
    // config = wickonce(config, all_allowed_pairings);
    // cout << config << endl;

    config = wickonceallpermutations(config, all_allowed_pairings);
    config = wickonceordered(config, all_allowed_pairings);
    config = wickonceordered(config, all_allowed_pairings);
    
    cout << "sunset diagram for standard phi4 theory: " << config.size() << endl;
    cout << "(4!)^2 / 6 == " << tgamma(5)*tgamma(5) / 6 << endl;
}


half_edge find_edge_pair(const half_edge& he, const state& s){
    for(auto& e : s.edges){
        if(he == get<0>(e)) 
            return get<1>(e);
        if(he == get<1>(e)) 
            return get<0>(e);
    }
    throw;
}
bool connected(const state& s){
    auto& vs = s.vertices;
    auto& es = s.edges;
    
    vector<bool> vis(vs.size(),false);
    
    int cnt = 1;
    int cur_vertex = 0;
    vis[0] = true;

    queue<int> q;
    q.push(0);
    while(!q.empty()){
        auto& v = q.front();
        q.pop();
        for(auto& he : vs[v]){
            if(!he.paired)
                continue;
            auto he_pairing = find_edge_pair(he,s);
            int next_vertex = s.m.at(he_pairing.id);
            if(!vis[next_vertex]){
                vis[next_vertex] = true;
                q.push(next_vertex);
            }
        }
    }
    for(auto x : vis)
        if(!x)
            return false;
    return true;
}
bool boson_diameter(const state& s, int N){
    auto& vs = s.vertices;
    auto& es = s.edges;

    int A,B;
    vector<bool> vis(vs.size(),false);
    for(auto& [a,b] : es){
        if(a.type == 0){
            A = s.m.at(a.id);
            B = s.m.at(b.id);
            break;
        }
    }

    vis[A] = true;
    vis[B] = true;

    int cnt = 0;
    int cur_vertex = A;
    while(cur_vertex != B){
        for(auto& he : vs[cur_vertex]){
            if(he.type != 1)
                continue;
            cur_vertex = s.m.at(find_edge_pair(he, s).id);
            cnt++;
            break;
        }
    }
    return cnt == N;
}
void solve2(){ // four boson correction, two loop (fermion loop)
    vector<half_edge> vertex1 = { {0,0,0}, {1,1,0}, {2,-1,0} };
    vector<half_edge> vertex2 = { {3,0,0}, {4,1,0}, {5,-1,0} };
    vector<half_edge> vertex3 = { {6,0,0}, {7,1,0}, {8,-1,0} };
    vector<half_edge> vertex4 = { {9,0,0}, {10,1,0}, {11,-1,0} };
    vector<half_edge> vertex5 = { {12,0,0}, {13,1,0}, {14,-1,0} };
    vector<half_edge> vertex6 = { {15,0,0}, {16,1,0}, {17,-1,0} };

    vector<vertex> vertices_init = {vertex1,vertex2,vertex3,vertex4,vertex5,vertex6};
    vector<state> config = {  {  vertices_init, vector<edge>(), gen_map(vertices_init)  } };

    vector<tuple<int,int>> all_allowed_pairings  = { {0,0}, {1,-1}, {-1,1} };
    vector<tuple<int,int>> fermion_pairings_only = { {1,-1}, {-1,1} };
    vector<tuple<int,int>> boson_pairings_only   = { {0,0} };

    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    
    // keep connected fermion loops
    vector<state> modified_config;
    for(auto& s : config)
        if(connected(s))
            modified_config.push_back(s);
    
    // wick contract the boson
    config = wickonceallpermutations(modified_config, boson_pairings_only);
    modified_config = vector<state>();
    for(auto& s : config)
        if(boson_diameter(s,3))
            modified_config.push_back(s);
    
    cout  << "four boson, two loop correction: " << modified_config.size() << endl;
    cout  << "                         6! / 2: " << tgamma(7)/2 << endl;
}

bool internal_loop(const state& s){
    for(int i = 0; i < s.edges.size(); ++i){
        for(int j = i+1; j < s.edges.size(); ++j){
            auto& [a,b] = s.edges[i];
            auto& [c,d] = s.edges[j];
            if (a.type == c.type && b.type == d.type)
                return true;
        }
    }
    return false;
}
bool no_internal_loop(const state& s){
    for(int i = 0; i < s.edges.size(); ++i){
        for(int j = i+1; j < s.edges.size(); ++j){
            auto& [a,b] = s.edges[i];
            auto& [c,d] = s.edges[j];
            if (a.type == c.type && b.type == d.type)
                return false;
        }
    }
    return true;
}
bool half_internal_loop(const state& s){
    assert(s.edges.size() == 2);
    auto& [a,b] = s.edges[0];
    auto& [c,d] = s.edges[1];
    return (a.type == c.type && b.type != d.type) || (a.type != c.type && b.type == d.type);
}
bool pseudo_internal_loop(const state& s){
    assert(s.edges.size() == 2);
    auto& [a,b] = s.edges[0];
    auto& [c,d] = s.edges[1];
    return (a.type != c.type && b.type != d.type);
}

template<int N>
array< array<int,2>, N > compress_state(const state& s){
    assert(N == s.edges.size());
    array<array<int,2>,N> A;
    for(int i = 0; i < N; ++i){
        auto& [a,b] = s.edges[i];
        for(int k = 0; k < s.vertices[0].size(); ++k)
            if(s.vertices[0][k].id == a.id)
                A[i][0] = k;
        for(int k = 0; k < s.vertices[1].size(); ++k)
            if(s.vertices[1][k].id == b.id)
                A[i][1] = k;
    }
    sort(A.begin(), A.end());
    return A;
}

void solve3(){ //sunset diagram (modified phi4) part 1
    vertex vertex1 = { {0,1,0}, {1,1,0}, {2,2,0}, {3,2,0} };
    vertex vertex2 = { {4,1,0}, {5,1,0}, {6,2,0}, {7,2,0} };

    vector<state> config = {  {  {vertex1, vertex2}, vector<edge>(), gen_map({vertex1,vertex2})  } };
    vector<tuple<int,int>> all_allowed_pairings  = { {1,1}, {1,2}, {2,1}, {2,2} };
    
    config = wickonceallpermutations(config, all_allowed_pairings);
    config = wickonceallpermutations(config, all_allowed_pairings);
    config = wickonceallpermutations(config, all_allowed_pairings);

    vector< array<array<int,2>,3> > modified_config;
    for(auto& s : config)
        if(internal_loop(s))
            modified_config.push_back(compress_state<3>(s));
    
    sort(modified_config.begin(), modified_config.end());
    auto last = unique(modified_config.begin(), modified_config.end());
    modified_config.erase(last, modified_config.end());

    cout << "sunset diagram Part 1 --- Modified phi4: " << modified_config.size() << endl;
    for(int i = 0; i < modified_config.size(); ++i){
        auto& [x,y,z] = modified_config[i];
        cout << "(" << x[0] << " ->- " << x[1] << ")  (" << y[0] << " ->- " << y[1] << ")  (" << z[0] << " ->- " << z[1] << ")" << endl;
    }
}
void solve4(){ //sunset diagram (modified phi4) part 2
    vertex vertex1 = { {0,1,0}, {1,1,0}, {2,2,0}, {3,2,0} };
    vertex vertex2 = { {4,1,0}, {5,1,0}, {6,2,0}, {7,2,0} };

    vector<state> config = {  {  {vertex1, vertex2}, vector<edge>(), gen_map({vertex1,vertex2})  } };
    vector<tuple<int,int>> all_allowed_pairings  = { {1,1}, {1,2}, {2,1}, {2,2} };
    
    config = wickonceallpermutations(config, all_allowed_pairings);
    config = wickonceallpermutations(config, all_allowed_pairings);
    config = wickonceallpermutations(config, all_allowed_pairings);

    // keep connected fermion loops
    vector< array<array<int,2>,3> > modified_config;
    for(auto& s : config)
        if(no_internal_loop(s))
            modified_config.push_back(compress_state<3>(s));

    sort(modified_config.begin(), modified_config.end());
    auto last = unique(modified_config.begin(), modified_config.end());
    modified_config.erase(last, modified_config.end());

    cout << "sunset diagram Part 2 --- Modified phi4: " << modified_config.size() << endl;
    for(int i = 0; i < modified_config.size(); ++i){
        auto& [x,y,z] = modified_config[i];
        cout << "(" << x[0] << " ->- " << x[1] << ")  (" << y[0] << " ->- " << y[1] << ")  (" << z[0] << " ->- " << z[1] << ")" << endl;
    }
}

void solve5(){ // boson self energy, two loop (fermion loop)
    vector<half_edge> vertex1 = { {0,0,0}, {1,1,0}, {2,-1,0} };
    vector<half_edge> vertex2 = { {3,0,0}, {4,1,0}, {5,-1,0} };
    vector<half_edge> vertex3 = { {6,0,0}, {7,1,0}, {8,-1,0} };
    vector<half_edge> vertex4 = { {9,0,0}, {10,1,0}, {11,-1,0} };

    vector<vertex> vertices_init = {vertex1,vertex2,vertex3,vertex4};
    vector<state> config = {  {  vertices_init, vector<edge>(), gen_map(vertices_init)  } };

    vector<tuple<int,int>> all_allowed_pairings  = { {0,0}, {1,-1}, {-1,1} };
    vector<tuple<int,int>> fermion_pairings_only = { {1,-1}, {-1,1} };
    vector<tuple<int,int>> boson_pairings_only   = { {0,0} };

    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    
    // keep connected fermion loops
    vector<state> modified_config;
    for(auto& s : config)
        if(connected(s))
            modified_config.push_back(s);
    
    cout << "after contracting just the fermion loop == " << modified_config.size() << endl;

    // wick contract the boson
    config = wickonceallpermutations(modified_config, boson_pairings_only);
    modified_config = vector<state>();
    for(auto& s : config)
        if(boson_diameter(s,2))
            modified_config.push_back(s);
    
    cout  << "boson self energy, two loop (fermion loop): " << modified_config.size() << endl;
    cout  << "symmetry factor is then (1/4!) * " << modified_config.size() << " == " << modified_config.size()/double(tgamma(5)) << endl;
}
void solve6(){ // one loop boson self energy
    vector<half_edge> vertex1 = { {0,0,0}, {1,1,0}, {2,-1,0} };
    vector<half_edge> vertex2 = { {3,0,0}, {4,1,0}, {5,-1,0} };

    vector<vertex> vertices_init = {vertex1,vertex2};
    vector<state> config = {  {  vertices_init, vector<edge>(), gen_map(vertices_init)  } };

    vector<tuple<int,int>> all_allowed_pairings  = { {0,0}, {1,-1}, {-1,1} };
    vector<tuple<int,int>> fermion_pairings_only = { {1,-1}, {-1,1} };
    vector<tuple<int,int>> boson_pairings_only   = { {0,0} };

    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceallpermutations(config, boson_pairings_only);
    
    cout << "one loop boson self energy: " << config.size() << endl;
    cout << "symmetry factor: (1/2!) * " << config.size() << " = " << config.size()/2.0 << endl;
}

void solve7(){ // four boson correction, one fermion loop
    vector<half_edge> vertex1 = { {0,0,0}, {1,1,0}, {2,-1,0} };
    vector<half_edge> vertex2 = { {3,0,0}, {4,1,0}, {5,-1,0} };
    vector<half_edge> vertex3 = { {6,0,0}, {7,1,0}, {8,-1,0} };
    vector<half_edge> vertex4 = { {9,0,0}, {10,1,0}, {11,-1,0} };

    vector<vertex> vertices_init = {vertex1,vertex2,vertex3,vertex4};
    vector<state> config = {  {  vertices_init, vector<edge>(), gen_map(vertices_init)  } };

    vector<tuple<int,int>> all_allowed_pairings  = { {0,0}, {1,-1}, {-1,1} };
    vector<tuple<int,int>> fermion_pairings_only = { {1,-1}, {-1,1} };
    vector<tuple<int,int>> boson_pairings_only   = { {0,0} };

    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    
    // keep connected fermion loops
    vector<state> modified_config;
    for(auto& s : config)
        if(connected(s))
            modified_config.push_back(s);
    
    cout  << "four boson correction, one fermion loop: " << modified_config.size() << endl;
    cout  << "                                 4! / 4: " << tgamma(5)/4 << endl;
}

void solve8(){ // four boson correction, one boson loop (modified phi4) part 1
    vertex vertex1 = { {0,1,0}, {1,1,0}, {2,2,0}, {3,2,0} };
    vertex vertex2 = { {4,1,0}, {5,1,0}, {6,2,0}, {7,2,0} };

    vector<state> config = {  {  {vertex1, vertex2}, vector<edge>(), gen_map({vertex1,vertex2})  } };
    vector<tuple<int,int>> all_allowed_pairings  = { {1,1}, {1,2}, {2,1}, {2,2} };
    
    config = wickonceallpermutations(config, all_allowed_pairings);
    config = wickonceallpermutations(config, all_allowed_pairings);

    // keep connected fermion loops
    vector< array<array<int,2>,2> > modified_config;
    for(auto& s : config)
        if(internal_loop(s))
            modified_config.push_back(compress_state<2>(s));

    sort(modified_config.begin(), modified_config.end());
    auto last = unique(modified_config.begin(), modified_config.end());
    modified_config.erase(last, modified_config.end());

    cout << "four boson correction, one boson loop part 1 --- Modified phi4: " << modified_config.size() << endl;
    for(int i = 0; i < modified_config.size(); ++i){
        auto& [x,y] = modified_config[i];
        cout << "(" << x[0] << " ->- " << x[1] << ")  (" << y[0] << " ->- " << y[1] << ")" << endl;
    }
}
void solve9(){ // four boson correction, one boson loop (modified phi4) part 2
    vertex vertex1 = { {0,1,0}, {1,1,0}, {2,2,0}, {3,2,0} };
    vertex vertex2 = { {4,1,0}, {5,1,0}, {6,2,0}, {7,2,0} };

    vector<state> config = {  {  {vertex1, vertex2}, vector<edge>(), gen_map({vertex1,vertex2})  } };
    vector<tuple<int,int>> all_allowed_pairings  = { {1,1}, {1,2}, {2,1}, {2,2} };
    
    config = wickonceallpermutations(config, all_allowed_pairings);
    config = wickonceallpermutations(config, all_allowed_pairings);

    // keep connected fermion loops
    vector< array<array<int,2>,2> > modified_config;
    for(auto& s : config)
        if(half_internal_loop(s))
            modified_config.push_back(compress_state<2>(s));

    sort(modified_config.begin(), modified_config.end());
    auto last = unique(modified_config.begin(), modified_config.end());
    modified_config.erase(last, modified_config.end());

    cout << "four boson correction, one boson loop part 2 --- Modified phi4: " << modified_config.size() << endl;
    for(int i = 0; i < modified_config.size(); ++i){
        auto& [x,y] = modified_config[i];
        cout << "(" << x[0] << " ->- " << x[1] << ")  (" << y[0] << " ->- " << y[1] << ")" << endl;
    }

}
void solve10(){ // four boson correction, one boson loop (modified phi4) part 3
    vertex vertex1 = { {0,1,0}, {1,1,0}, {2,2,0}, {3,2,0} };
    vertex vertex2 = { {4,1,0}, {5,1,0}, {6,2,0}, {7,2,0} };

    vector<state> config = {  {  {vertex1, vertex2}, vector<edge>(), gen_map({vertex1,vertex2})  } };
    vector<tuple<int,int>> all_allowed_pairings  = { {1,1}, {1,2}, {2,1}, {2,2} };
    
    config = wickonceallpermutations(config, all_allowed_pairings);
    config = wickonceallpermutations(config, all_allowed_pairings);

    // keep connected fermion loops
    vector< array<array<int,2>,2> > modified_config;
    for(auto& s : config)
        if(pseudo_internal_loop(s))
            modified_config.push_back(compress_state<2>(s));

    sort(modified_config.begin(), modified_config.end());
    auto last = unique(modified_config.begin(), modified_config.end());
    modified_config.erase(last, modified_config.end());

    cout << "four boson correction, one boson loop part 3 --- Modified phi4: " << modified_config.size() << endl;
    for(int i = 0; i < modified_config.size(); ++i){
        auto& [x,y] = modified_config[i];
        cout << "(" << x[0] << " ->- " << x[1] << ")  (" << y[0] << " ->- " << y[1] << ")" << endl;
    }

}


void solve11(){ // yukawa
    vector<half_edge> vertex1 = { {0,0,0}, {1,1,0}, {2,-1,0} };
    vector<half_edge> vertex2 = { {3,0,0}, {4,1,0}, {5,-1,0} };
    vector<half_edge> vertex3 = { {6,0,0}, {7,1,0}, {8,-1,0} };

    vector<vertex> vertices_init = {vertex1,vertex2,vertex3};
    vector<state> config = {  {  vertices_init, vector<edge>(), gen_map(vertices_init)  } };

    vector<tuple<int,int>> all_allowed_pairings  = { {0,0}, {1,-1}, {-1,1} };
    vector<tuple<int,int>> fermion_pairings_only = { {1,-1}, {-1,1} };
    vector<tuple<int,int>> boson_pairings_only   = { {0,0} };

    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceordered(config, fermion_pairings_only);
    config = wickonceallpermutations(config, boson_pairings_only);
    
    // applying wickonceordered twice ==> vertex1 is the middle vertex
    vector<state> modified_config;
    array<int,3> vis;
    for(auto& s : config){
        vis = {0,0,0};
        for(auto& e : s.edges){
            auto& [b1,b2] = e;
            ++vis[ s.m[b1.id] ];
            ++vis[ s.m[b2.id] ];
        }
        if(vis[0] == 2 && vis[1] == 2 && vis[2] == 2)
            modified_config.push_back(s);
    }
        
    cout  << "yukawa: " << 3*modified_config.size() << endl;
    cout  << "    3!: " << tgamma(4) << endl;
}

int main()
{
    solve1(); // standard phi4 sunset diagram

    // solve2();

    // solve3(); // two loop boson self energy [boson loop] Part 1
    // solve4(); // two loop boson self energy [boson loop] Part 2

    // solve5(); // two loop boson self energy [fermion loop]
    // solve6(); // one loop boson self energy
    
    // solve7();

    // solve8();
    // solve9();
    // solve10();

    // solve11();

    return 0;
}