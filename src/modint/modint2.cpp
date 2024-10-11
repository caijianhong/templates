template<unsigned P> struct modint{
    unsigned v; modint():v(0){}
    template<class T> modint(T x):v((x%int(P)+int(P))%int(P)){}
    modint operator-()const{return modint(P-v);}
    modint inv()const{return qpow(*this,LL(P)-2);}
    modint&operator+=(const modint&rhs){if(v+=rhs.v,v>=P) v-=P; return *this;}
    modint&operator-=(const modint&rhs){return *this+=-rhs;}
    modint&operator*=(const modint&rhs){v=1ull*v*rhs.v%P; return *this;}
    modint&operator/=(const modint&rhs){return *this*=rhs.inv();}
    friend int raw(const modint&self){return self.v;}
    friend modint qpow(modint a,LL b){modint r=1;for(;b;b>>=1,a*=a) if(b&1) r*=a; return r;}
    friend modint operator+(modint lhs,const modint&rhs){return lhs+=rhs;}
    friend modint operator-(modint lhs,const modint&rhs){return lhs-=rhs;}
    friend modint operator*(modint lhs,const modint&rhs){return lhs*=rhs;}
    friend modint operator/(modint lhs,const modint&rhs){return lhs/=rhs;}
    friend bool operator==(const modint&lhs,const modint&rhs){return lhs.v==rhs.v;}
    friend bool operator!=(const modint&lhs,const modint&rhs){return lhs.v!=rhs.v;}
};
