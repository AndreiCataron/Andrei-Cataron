#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <time.h>

using namespace std;

class Item{
protected:
    string nume;
    double pret_brut;
    double pret_in_meniu;
    int cantitateml;
    double alcoolemie;
public:
	Item(){};///constructor gol
    Item(string, double, double, int, double);///constructor cu parametrii
    Item(const Item&);///constructor de copiere
    Item& operator=(Item&);///operator de atribuire

    virtual ostream& print(ostream&) = 0;
    friend ostream& operator<<(ostream&, const Item&);

    virtual bool contine_alcool() = 0;

    ///getteri
    string get_nume(){return nume;}
    double get_pret_brut(){return pret_brut;}
    double get_pret_in_meniu(){return pret_in_meniu;}
    int get_cantitateml(){return cantitateml;}
    double get_alcoolemie(){return alcoolemie;}
};

class Ingredient{
private:
    char* nume;
    double pret_pe_litru;
    double alcoolemie;
public:
    Ingredient();///constructor cu nimic
    Ingredient(char*, double, double);///constructor cu parametrii
    Ingredient(const Ingredient&);///constructor de copiere
    Ingredient& operator=(Ingredient&);///operator de atribuire
    ~Ingredient();///destructor

    friend ostream& operator<<(ostream &, Ingredient&);///afisare

    bool operator< (const Ingredient& ing) const ///suprascriere '<' ca sa pot avea Ingredient ca key in map
    {
        return nume < ing.nume;
    }

    ///getteri si setteri
    char* get_nume(){return this->nume;}
    double get_pret_pe_litru(){return this->pret_pe_litru;}
    double get_alcoolemie(){return this->alcoolemie;}
    void set_nume(char* n){
        if(this->nume != NULL){
            delete[] this->nume;
        }
        this->nume = new char[strlen(n)+1];
        strcpy(this->nume, n);
    }
    void set_pret(double p){this->pret_pe_litru = p;}
    void set_alcoolemie(double a){this->alcoolemie = a;}
};

class Cocktail: public Item{
private:
    map<Ingredient, int> ingrediente;///perechi de forma (Ingredient, procent din Cocktail)
public:
    Cocktail(){};///constructor cu nimic
    Cocktail(string, map<Ingredient, int>, int, double);///constructor cu parametrii
    Cocktail(const Cocktail&); ///constructor de copiere
    Cocktail& operator=(Cocktail&); ///operator de atribuire

    ostream& print(ostream&);
    Ingredient operator[](int);///returneaza ingredint din lista de ingrediente

    ///////

    bool contine_alcool();
    bool verifica_cantitati();///verifica daca suma procentelor ingredientelor este 100
    void calculeaza_alcoolemia();///calculeaza alcoolemia bauturii din alcoolemiile ingredientelor
    void calculeaza_pret_ingrediente();///calculeaza pretul ingredientelor necesare pentru un Cocktail
};

class Sticla_Vin : public Item{
private:
    string culoare;
    string gust;
    string soi_struguri;
    int an;
    int vechime;
public:
    Sticla_Vin(){};///constructor cu nimic
    Sticla_Vin(string, double, double, double, string, string, string, int);///constructor cu parametrii
    Sticla_Vin(const Sticla_Vin&);///constructor de copiere
    Sticla_Vin& operator=(Sticla_Vin&);///operator de atribuire

    Sticla_Vin operator++(int);///adauga un an la vechimea vinului
    Sticla_Vin operator+=(int);///adauga un numar de ani la vechimea vinului

    ostream& print(ostream&);

    bool contine_alcool();

};

class Pahar_Vin : public Sticla_Vin{
    double pret_pahar;
public:
    Pahar_Vin(const Sticla_Vin&);///constructor care primeste vinul ca parametru si apeleaza copy constructorul lui Vin
    void calc_pret(){this->pret_pahar = pret_in_meniu / 5.;}

    ///getteri si setteri
    double get_pret_pahar(){return pret_pahar;}
};

class Comanda{
private:
    vector<Item*> v;
    static int nr_total_produse_comandate;
    int nr_produse_pe_comanda;
    double pret_total = 0;
public:
    Comanda(){};
    Comanda(Item&);///constructor cu parametru item

    Comanda& operator+=(Item&);///adaugarea unui item pe comanda
    Item* operator[](int);///returneaza un pointer la un element din v
    Comanda& operator-();///sterge ultimul item din comanda

    bool operator<(Comanda&);
    bool operator>(Comanda&);
    bool operator<=(Comanda&);
    bool operator>=(Comanda&);
    bool operator==(Comanda&);

    friend ostream& operator<<(ostream &, Comanda&);///afisare

    void reset_nr_total(){nr_total_produse_comandate = 0;}
    ///getteri si setteri
    int get_nr_total_produse(){return nr_total_produse_comandate;}
    int get_nr_produse(){return nr_produse_pe_comanda;}
    double get_pret_total(){return pret_total;}

};

//////////////////////////////
///Item

Item::Item(string n, double pb, double pm, int ml, double a){
	this->nume = n;
	this->pret_brut = pb;
	this->pret_in_meniu = pm;
	this->cantitateml = ml;
	this->alcoolemie = a;
}

Item::Item(const Item& ob){
	this->nume = ob.nume;
	this->pret_brut = ob.pret_brut;
	this->pret_in_meniu = ob.pret_in_meniu;
	this->cantitateml = ob.cantitateml;
	this->alcoolemie = ob.alcoolemie;
}

Item& Item::operator=(Item& ob){
	this->nume = ob.nume;
	this->pret_brut = ob.pret_brut;
	this->pret_in_meniu = ob.pret_in_meniu;
	this->cantitateml = ob.cantitateml;
	this->alcoolemie = ob.alcoolemie;
	return *this;
}

ostream& operator<<(ostream& out, Item& ob){
	ob.print(out);
	return out;
}

////////////////////////////////////////////////////////
///Ingredient

Ingredient::Ingredient(){
    this->nume = new char[strlen("noname")+1];
    strcpy(this->nume, "noname");
    this->pret_pe_litru = 0;
    this->alcoolemie = 0;
}

Ingredient::Ingredient(char* s, double p, double a){
    this->nume = new char[strlen(s)+1];
    strcpy(this->nume, s);
    this->pret_pe_litru = p;
    this->alcoolemie = a;
}

Ingredient::Ingredient(const Ingredient& ing)
{
    nume = new char[strlen(ing.nume)+1];
    strcpy(nume, ing.nume);
    pret_pe_litru = ing.pret_pe_litru;
    alcoolemie = ing.alcoolemie;
}

Ingredient::~Ingredient(){
    if(this->nume != NULL){
        delete[] this->nume;
    }
}

Ingredient& Ingredient::operator=(Ingredient& ing)
{
    if (this != &ing)
    {
        if(nume != NULL){
            delete[] nume;
        }
        nume = new char[strlen(ing.nume)+1];
        strcpy(nume, ing.nume);
        pret_pe_litru = ing.pret_pe_litru;
        alcoolemie = ing.alcoolemie;
    }
    return *this;
}

ostream& operator<<(ostream& out, Ingredient& ing)
{
    out<<ing.get_nume();
    return out;
}

///////////////////////////////////////////
///Cocktail

Cocktail::Cocktail(string s, map<Ingredient, int> m, int ml, double p): Item(s, 0, p, ml, 0){
    this->ingrediente = m;
}

Cocktail::Cocktail(const Cocktail& ob):Item(ob)
{
    ingrediente = ob.ingrediente;
}

Cocktail& Cocktail::operator=(Cocktail& ob)
{
    if (this != &ob)
    {
        Item::operator=(ob);
        ingrediente = ob.ingrediente;
    }
    return *this;
}

ostream& Cocktail::print(ostream& out){
    map<Ingredient, int>::iterator itr;
    out<<"Cocktail-ul "<<this->nume<<" contine: ";
    for(itr = this->ingrediente.begin(); itr != this->ingrediente.end(); itr++){
        Ingredient temp = (*itr).first;
        out<<temp.get_nume()<<", ";
    }
    out<<" in procentele:";
    for(itr = this->ingrediente.begin(); itr != this->ingrediente.end(); itr++){
        out<<(*itr).second<<", ";
    }
    out<<endl;
    return out;
}

Ingredient Cocktail::operator[](int i){
    map<Ingredient, int>::iterator itr = ingrediente.begin();
    for(int j = 0; j < i; j++){
        itr++;
    }
    Ingredient temp = (*itr).first;
    return temp;
}

bool Cocktail::contine_alcool(){
    return this->alcoolemie != 0;
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

void Cocktail::calculeaza_pret_ingrediente(){
    double p = 0;
    map<Ingredient, int>::iterator itr;
    for(itr = this->ingrediente.begin(); itr != this->ingrediente.end(); itr++){
        Ingredient temp = (*itr).first;
        p += temp.get_pret_pe_litru() * (*itr).second / 100;
    }
    this->pret_brut = p * (this->cantitateml) / 1000.;
}

////////////////////////////////////////////////
///Sticla_Vin

Sticla_Vin::Sticla_Vin(string n, double pb, double pm, double a, string c, string g, string s, int an):Item(n, pb, pm, 750, a)
{
    this->culoare = c;
    this->gust = g;
    this->soi_struguri = s;
    this->an = an;
    time_t t = time(NULL);
    tm* timpptr = localtime(&t);
    this->vechime = timpptr->tm_year - this->an + 1900;
}

Sticla_Vin::Sticla_Vin(const Sticla_Vin& v):Item(v)
{
    this->culoare = v.culoare;
    this->gust = v.gust;
    this->soi_struguri = v.soi_struguri;
    this->an = v.an;
    this->vechime = v.vechime;
}

Sticla_Vin& Sticla_Vin::operator=(Sticla_Vin& v)
{
    if (this != &v)
    {
        Item::operator=(v);
        this->culoare = v.culoare;
        this->gust = v.gust;
        this->soi_struguri = v.soi_struguri;
        this->an = v.an;
        this->vechime = v.vechime;
    }
    return *this;
}

Sticla_Vin Sticla_Vin::operator++(int dummy)
{
    vechime += 1;
    return *this;
}

Sticla_Vin Sticla_Vin::operator+=(int x){
    vechime += x;
    return *this;
}

ostream& Sticla_Vin::print(ostream& out){
    out<<this->nume<<" "<<this->culoare<<" "<<this->gust<<" "<<this->an<<endl;
    return out;
}

bool Sticla_Vin::contine_alcool(){
    return true;
}

///////////////////////////////////////////
///Pahar_Vin
Pahar_Vin::Pahar_Vin(const Sticla_Vin& v):Sticla_Vin(v){
	this->cantitateml = 150;
    this->calc_pret();
}

/////////////////////////////////////////////////
///Comanda

Comanda::Comanda(Item& i){
    this->v.push_back(&i);
    this->pret_total = i.get_pret_in_meniu();
    this->nr_produse_pe_comanda = 1;
    nr_total_produse_comandate++;
}

Comanda& Comanda::operator+=(Item& i){
    this->v.push_back(&i);
    this->pret_total += i.get_pret_in_meniu();
    this->nr_produse_pe_comanda++;
    nr_total_produse_comandate++;
    return *this;
}

Item* Comanda::operator[](int i){
    return this->v[i];
}

Comanda& Comanda::operator-(){
    if(this->v.size() == 0){
        return *this;
    }
    this->pret_total -= this->v[v.size()-1]->get_pret_in_meniu();
    this->v.pop_back();
    this->nr_produse_pe_comanda--;
    nr_total_produse_comandate--;
    return *this;
}

bool Comanda::operator<(Comanda& com){
    return this->pret_total < com.get_pret_total();
}

bool Comanda::operator>(Comanda& com){
    return this->pret_total > com.get_pret_total();
}

bool Comanda::operator<=(Comanda& com){
    return this->pret_total <= com.get_pret_total();
}

bool Comanda::operator>=(Comanda& com){
    return this->pret_total >= com.get_pret_total();
}

bool Comanda::operator==(Comanda& com){
    return this->pret_total == com.get_pret_total();
}

ostream& operator<<(ostream& out, Comanda& com)
{
    out<<"Comanda:"<<endl;
    for(int i = 0; i < com.nr_produse_pe_comanda; i++){
        out<<com.v[i]->get_nume()<<": "<<com.v[i]->get_pret_in_meniu()<<endl;
    }
    out<<"Total: "<<com.pret_total;
    return out;
}

int Comanda::nr_total_produse_comandate = 0;

int main()
{
    Sticla_Vin chardonnay("Chardonnay Domeniile Samburesti", 25, 29.5, 12.5, "alb", "sec", "chardonnay", 2018);
    Pahar_Vin pahar_chardonnay(chardonnay);
    pahar_chardonnay.calc_pret();
    Ingredient vodca("vodca", 40, 40);
    Ingredient apa("apa", 2, 0);
    Ingredient lichior_lamaie("Lichior lamaie", 50, 22);
    Ingredient suc_lamaie("suc lamaie", 20, 0);
    map<Ingredient, int> ingrediente_kamikaze = {{vodca, 40}, {lichior_lamaie, 40}, {suc_lamaie, 20}};
    Cocktail kamikaze("kamikaze", ingrediente_kamikaze, 50, 7);
    for(int i = 0; i < 3; i++){
        Ingredient ing = kamikaze[i];
        cout<<ing<<endl;
    }
    Comanda c;
    c+=kamikaze;
    c+=chardonnay;
    cout<<c;

    return 0;
}
