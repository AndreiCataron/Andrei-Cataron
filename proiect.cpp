#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class Ingredient{
protected:
    string nume;
    double pret_pe_litru;
    double alcoolemie;
public:
    Ingredient(){};///constructor cu nimic
    Ingredient(string, double, double);///constructor cu parametrii
    Ingredient(const Ingredient&);///constructor de copiere
    Ingredient& operator=(Ingredient&);///operator de atribuire
    friend ostream& operator<<(ostream &, const Ingredient&);///afisare
    bool operator< (const Ingredient& ing) const ///suprascriere '<' ca sa pot avea Ingredient ca key in map
    {
        return nume < ing.nume;
    }

    ///getteri si setteri
    string get_nume(){return nume;}
    double get_pret(){return pret_pe_litru;}
    double get_alcoolemie(){return alcoolemie;}
};

class Cocktail{
private:
    string nume;
    double pret;
    int ml;
    double alcoolemie;
    map<Ingredient, int> ingrediente;///perechi de forma (Ingredient, procent din Cocktail)
public:
    Cocktail(){};///constructor cu nimic
    Cocktail(string, map<Ingredient, int>, int, double);///constructor cu parametrii
    Cocktail(const Cocktail&); ///constructor de copiere
    Cocktail& operator=(Cocktail&); ///operator de atribuire
    friend ostream& operator<<(ostream &, Cocktail&);///afisare

    ///////

    bool verifica_cantitati();///verifica daca suma procentelor ingredientelor este 100
    void calculeaza_alcoolemia();///calculeaza alcoolemia bauturii din alcoolemiile ingredientelor
    double pret_ingrediente();///calculeaza pretul ingredientelor necesare pentru un Cocktail

    ///getteri si setteri
    string get_nume(){return nume;}
    double get_pret(){return pret;}
    void set_nume(string s){nume = s;}
};

class Vin : public Ingredient{
protected:
    string culoare;
    string gust;
    string soi_struguri;
    int vechime;
    double pret_sticla;
public:
    Vin(){};///constructor cu nimic
    Vin(string, double, double, string, string, string, int, double);///constructor cu parametrii
    Vin(const Vin&);///constructor de copiere
    Vin& operator=(Vin&);///operator de atribuire
    Vin operator++(int);///adauga un an la vechimea vinului

    ///getteri si setteri
    double get_pret_sticla(){return pret_sticla;}
    void set_pret_sticla(double p){pret_sticla = p;}
};

class Pahar_vin : public Vin{
    double pret_pahar;
public:
    using Vin::Vin;///mostenirea constructorilor de la Vin
    void pret(){pret_pahar = pret_sticla / 5.;}

    ///getteri si setteri
    double get_pret_pahar(){return pret_pahar;}
};

class Comanda{
private:
    map< string, double> comanda;
    map< string, double>::iterator itr = comanda.begin();
    double pret_total = 0;
public:
    Comanda(){};///constructor cu nimic
    Comanda& operator+=(Cocktail&);///adaugarea unui cocktail pe comanda
    Comanda& operator+=(Vin&);///adaugarea unui vin pe comanda
    Comanda& operator+=(Pahar_vin&);///adaugarea unui pahar de vin pe comanda
    friend ostream& operator<<(ostream &, Comanda&);///afisare

    ///getteri si setteri
    map< string, double>::iterator get_itr(){return itr;}
};
//////////////////////////////////////////////////////////

Ingredient::Ingredient(string s, double p, double a){
    this->nume = s;
    this->pret_pe_litru = p;
    this->alcoolemie = a;
}

Ingredient::Ingredient(const Ingredient& ing)
{
    nume = ing.nume;
    pret_pe_litru = ing.pret_pe_litru;
    alcoolemie = ing.alcoolemie;
}

Ingredient& Ingredient::operator=(Ingredient& ing)
{
    if (this != &ing)
    {
        nume = ing.nume;
        pret_pe_litru = ing.pret_pe_litru;
        alcoolemie = ing.alcoolemie;
    }
    return *this;
}

ostream& operator<<(ostream& out, const Ingredient& ing)
{
    out<<ing.nume;
    return out;
}



///////////////////////////////////////////

Cocktail::Cocktail(string s, map<Ingredient, int> m, int ml, double p){
    this->nume = s;
    this->ingrediente = m;
    this->pret = p;
    this->ml = ml;
}

Cocktail::Cocktail(const Cocktail& ob)
{
    pret = ob.pret;
    nume = ob.nume;
    ml = ob.ml;
    alcoolemie = ob.alcoolemie;
    ingrediente = ob.ingrediente;
}

Cocktail& Cocktail::operator=(Cocktail& ob)
{
    if (this != &ob)
    {
        pret = ob.pret;
        nume = ob.nume;
        ml = ob.ml;
        alcoolemie = ob.alcoolemie;
        ingrediente = ob.ingrediente;
    }
    return *this;
}

ostream& operator<<(ostream& out, Cocktail& b)
{
    out<<"Nume Cocktail:"<<b.nume<<endl<<"Ingrediente: ";
    map<Ingredient, int>::iterator itr;
    for(itr = b.ingrediente.begin(); itr != b.ingrediente.end(); itr++){
        out<< (*itr).first;
        out<<" "<<itr->second<<"; ";
    }
    out<<endl;
    out<<"Pret: "<<b.pret<<endl;
    out<<"Cantitate(ml): "<<b.ml<<endl;
    out<<"Alcoolemie: "<<b.alcoolemie<<endl;

    return out;
}

bool Cocktail::verifica_cantitati(){
    int s = 0;
    map<Ingredient, int>::iterator itr;
    for(itr = this->ingrediente.begin(); itr != this->ingrediente.end(); itr++){
        s += (*itr).second;
    }
    if(s == 100){
        return true;
    }
    return false;
}

void Cocktail::calculeaza_alcoolemia(){
    double s = 0;
    map<Ingredient, int>::iterator itr;
    for(itr = this->ingrediente.begin(); itr != this->ingrediente.end(); itr++){
        Ingredient temp = (*itr).first;
        s += temp.get_alcoolemie() * (*itr).second / 100;
    }
    this->alcoolemie = s;
}

double Cocktail::pret_ingrediente(){
    double p = 0;
    map<Ingredient, int>::iterator itr;
    for(itr = this->ingrediente.begin(); itr != this->ingrediente.end(); itr++){
        Ingredient temp = (*itr).first;
        p += temp.get_pret() * (*itr).second / 100;
    }
    return p * (this->ml) / 1000.;
}

////////////////////////////////////////////////

Vin Vin::operator++(int dummy)
{
    vechime += 1;
    return *this;
}

Vin::Vin(string n, double pl, double a, string c, string g, string s, int v, double ps)
{
    this->nume = n;
    this->pret_pe_litru = pl;
    this->alcoolemie = a;
    this->culoare = c;
    this->gust = g;
    this->soi_struguri = s;
    this->vechime = v;
    this->pret_sticla = ps;
}

Vin::Vin(const Vin& v)
{
    nume = v.nume;
    pret_pe_litru = v.pret_pe_litru;
    alcoolemie = v.alcoolemie;
    culoare = v.culoare;
    gust = v.gust;
    soi_struguri = v.soi_struguri;
    vechime = v.vechime;
    pret_sticla = v.pret_sticla;
}

Vin& Vin::operator=(Vin& v)
{
    if (this != &v)
    {
        nume = v.nume;
        pret_pe_litru = v.pret_pe_litru;
        alcoolemie = v.alcoolemie;
        culoare = v.culoare;
        gust = v.gust;
        soi_struguri = v.soi_struguri;
        vechime = v.vechime;
        pret_sticla = v.pret_sticla;
    }
    return *this;
}

/////////////////////////////////////////////////

Comanda& Comanda::operator+=(Cocktail& c)
{
    map< string, double>::iterator it = this->get_itr();
    this->comanda.insert(it, pair< string, double>(c.get_nume(), c.get_pret()));
    this->pret_total += c.get_pret();
    return *this;
}

Comanda& Comanda::operator+=(Vin& v)
{
    map< string, double>::iterator it = this->get_itr();
    this->comanda.insert(it, pair< string, double>(v.get_nume(), v.get_pret_sticla()));
    this->pret_total += v.get_pret_sticla();
    return *this;
}

Comanda& Comanda::operator+=(Pahar_vin& pv)
{
    map< string, double>::iterator it = this->get_itr();
    this->comanda.insert(it, pair< string, double>(pv.get_nume(), pv.get_pret_pahar()));
    this->pret_total += pv.get_pret_pahar();
    return *this;
}

ostream& operator<<(ostream& out, Comanda& c)
{
    out<<"Comanda:"<<endl;
    map< string, double>::iterator itr;
    for(itr = c.comanda.begin(); itr != c.comanda.end(); itr++){
        out<< (*itr).first;
        out<<" "<<itr->second<<endl;
    }
    out<<"Total: "<<c.pret_total;
    return out;
}

int main()
{
    map< Ingredient, int> ingrediente_1;
    Ingredient apa("apa", 10, 15);
    Ingredient vodca("vodca", 35, 20);

    map< Ingredient, int>::iterator it = ingrediente_1.begin();
    ingrediente_1.insert(it, pair<Ingredient, int>(apa, 60));
    ingrediente_1.insert(it, pair<Ingredient, int>(vodca, 40));
    Cocktail b("vodca cu apa", ingrediente_1, 23, 55);
    b.calculeaza_alcoolemia();
    cout << b;
    cout << b.verifica_cantitati();
    Comanda com;
    com += b;
    cout<<com;
    Vin vin("Chardonnay Domeniile Samburesti", 25, 12.5, "alb", "sec", "chardonnay", 2, 30);
    com += vin;
    cout<<endl<<com;
    Pahar_vin p("Chardonnay Domeniile Samburesti", 25, 12.5, "alb", "sec", "chardonnay", 2, 30);
    p.pret();
    cout<<endl<<p.get_pret_pahar();
    return 0;
}
