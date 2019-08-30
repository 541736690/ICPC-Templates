/*
 *  Dominator tree in O(M log(N)/log(2+M/N)) time, O(M+N) space
 *  Algorithm by T.Lengauer and R.E.Tarjan
 *  This is essentially the directed version of articulation points
 */

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = (int)1e5 + 50;

struct Dominator{
    struct min_DSU{
        vector<int> par, val;
        vector<int> const&semi;
        min_DSU(int N, vector<int> const&semi):par(N, -1),val(N), semi(semi){
            iota(val.begin(), val.end(), 0);
        }
        void comp(int x){
            if(par[par[x]]!=-1){
                comp(par[x]);
                if(semi[val[par[x]]]<semi[val[x]])
                    val[x] = val[par[x]];
                par[x]=par[par[x]];
            }
        }
        int f(int x){
            if(par[x]==-1) return x;
            comp(x);
            return val[x];
        }
        void link(int x, int p){
            par[x] = p;
        }
    };
    int N;
    vector<vector<int> > G, rG;
    vector<int> idom, order;
    Dominator(int _N):N(_N), G(N), rG(N){}
    void add_edge(int a, int b){
        G[a].emplace_back(b);
        rG[b].emplace_back(a);
    }
    vector<int> calc_dominators(int S){
        idom.assign(N, -1);
        vector<int> par(N, -1), semi(N, -1);
        vector<vector<int> > bu(N);
        stack<int> s;
        s.emplace(S);
        while(!s.empty()){
            int a=s.top();s.pop();
            if(semi[a]==-1){
                semi[a] = order.size();
                order.emplace_back(a);
                for(int i=0;i<(int)G[a].size();++i){
                    if(semi[G[a][i]]==-1){
                        par[G[a][i]]=a;
                        s.push(G[a][i]);
                    }
                }
            }
        }
        min_DSU uni(N, semi);
        for(int i=(int)order.size()-1;i>0;--i){
            int w=order[i];
            for(int f:rG[w]){
                int oval = semi[uni.f(f)];
                if(oval>=0 && semi[w]>oval) semi[w] = oval;
            }
            bu[order[semi[w]]].push_back(w);
            uni.link(w, par[w]);
            while(!bu[par[w]].empty()){
                int v = bu[par[w]].back(); bu[par[w]].pop_back();
                int u=uni.f(v);
                idom[v] = semi[u] < semi[v] ? u : par[w];
            }
        }
        for(int i=1;i<(int)order.size();++i){
            int w=order[i];
            if(idom[w] != order[semi[w]])
                idom[w] = idom[idom[w]];
        }
        idom[S]=-1;
        return idom;
    }
};

int n, m;
vector<int> par;
int sz[N];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    Dominator D(n);
    for(int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b; a--, b--;
        D.add_edge(a, b);
    }
    // Calc the immediate dominators for each node.
    par = D.calc_dominators(0);

    // D.order stores the topological order the the dominator tree. 0 is the root.
    // Only reachable nodes from the source will appear here.
    for(int i = 0; i < n; i++) sz[i] = 1;
    for(int i = (int)D.order.size() - 1; i > 0; i--) {
        int e = D.order[i];
        sz[par[e]] += sz[e];
    }

}