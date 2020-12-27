#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <time.h>
#include <set>
#include <list>
#include <algorithm>
#include <fstream>
#include <typeinfo>

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
    Cocktail(map<Ingredient, int>);///constructor cu un singur parametru
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
    void calc_pret(){this->pret_in_meniu = pret_in_meniu / 5.;}

    ostream& print(ostream&);

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
    char* numetemp = new char[100];
    strcpy(numetemp, ing.get_nume());
    out<<numetemp;
    return out;
}

///////////////////////////////////////////
///Cocktail

Cocktail::Cocktail(string s, map<Ingredient, int> m, int ml, double p): Item(s, 0, p, ml, 0){
    this->ingrediente = m;
}

Cocktail::Cocktail(map<Ingredient, int> m):Item("noname", 0, 0, 0, 0){
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
    out<<"Cocktail-ul "<<this->nume<<" (contine (ingredient:procent): ";
    for(itr = this->ingrediente.begin(); itr != --(this->ingrediente.end()); itr++){
        Ingredient temp = (*itr).first;
        out<<temp.get_nume()<<":"<<(*itr).second<<", ";
    }
    Ingredient temp = (*itr).first;
    out<<temp.get_nume()<<":"<<(*itr).second<<")";
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
    this->nume += " la pahar";
	this->cantitateml = 150;
    this->calc_pret();
}

ostream& Pahar_Vin::print(ostream& out){
    out<<this->get_nume();
    return out;
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
    list<Item*> meniu;
    set<Ingredient*> ingrediente;
    vector<Comanda*> comenzi;

    cout<<"---------------------------------------\n";
    cout<<"Meniu gestionare aplicatie bar \n";
    cout<<"---------------------------------------\n";
    cout<<"Alegeti una din urmatoarele optiuni: \n";
    cout<<"1 - adauga un item in meniu\n";
    cout<<"2 - adauga un ingredient pe lista de ingrediente\n";
    cout<<"3 - afiseaza meniul\n";
    cout<<"4 - afiseaza lista de ingrediente\n";
    cout<<"5 - adauga item in meniu dintr-un fisier\n";
    cout<<"6 - adauga itemuri in lista de ingrediente dintr-un fisier\n";
    cout<<"7 - salveaza meniul intr-un fisier\n";
    cout<<"8 - salveaza lista de ingrediente intr-un fisier\n";
    cout<<"9 - creeaza o comanda\n";
    cout<<"a - adaugati un item la o comanda\n";
    cout<<"b - afiseaza comenzile\n";
    cout<<"0 - inchideti programul\n";
    cout<<endl;

    char alegere = ' ';
    while(alegere != '0'){
        cout<<"\nAlegeti optiune: ";
        cin>>alegere;
        cout<<endl;
        switch(alegere){
        case '1':
            {
            cout<<"Alege ce tip de item (introdu litera corespunzatoare):\n";
            cout<<"v - Vin\n";
            cout<<"c - Creeaza un cocktail\n";
            cout<<"p - Pahar de vin\n";

            char alegere2 = ' ';
            cin>>alegere2;
            switch(alegere2){
            case 'v':
                {
                string nume;
                double pret_brut;
                double pret_meniu;
                double alcoolemie;
                string culoare;
                string gust;
                string soi;
                int an;
                cout<<"Introdu caracteristicile sticlei de vin:\n";
                cout<<"Nume: ";
                getline(cin>>ws, nume); //https://stackoverflow.com/questions/5838711/stdcin-input-with-spaces
                cout<<"Pret brut: ";
                cin>>pret_brut;
                cout<<"Pret meniu: ";
                cin>>pret_meniu;
                cout<<"Alcoolemie: ";
                cin>>alcoolemie;
                cout<<"Culoare: ";
                cin>>culoare;
                cout<<"Gust: ";
                cin>>gust;
                cout<<"Soi struguri: ";
                cin>>soi;
                cout<<"An: ";
                cin>>an;
                Sticla_Vin* sv = new Sticla_Vin(nume, pret_brut, pret_meniu, alcoolemie, culoare, gust, soi, an);
                meniu.push_front(sv);
                list<Item*>::iterator i = meniu.begin();
                cout<<"Ati introdus in meniu Sticla de Vin: "<<*(*i)<<endl;
                break;
                }
            case 'c':
            {
                cout<<"Ati ales sa adaugati un cocktail in meniu. Va trebui sa introduceti numarul de ingrediente si sa le selectati din lista de ingrediente, iar apoi sa specificati proportiile in care trebuie amestecate.\n";
                cout<<"Lista de ingrediente este: ";
                set<Ingredient*>::iterator it = ingrediente.begin();
                for(; it != ingrediente.end(); it++){
                    cout<<*(*it)<<" ";
                }
                cout<<endl;
                int nring;
                cout<<"Introduceti numarul de ingrediente: ";
                cin>>nring;
                cout<<"Introduceti numele ingredientelor (case-sensitive) urmate de procentul lor: ";
                string nume;
                int cant;
                map<Ingredient, int> map_ing;
                for(int i = 0; i < nring; i++){
                    getline(cin>>ws, nume);
                    char* n = new char[100];
                    size_t length = nume.copy(n, nume.size(), 0); //https://www.techiedelight.com/convert-string-char-array-cpp/
                    n[length] = '\0';
                    try{
                        set<Ingredient*>::iterator it = find_if(ingrediente.begin(), ingrediente.end(), [&](Ingredient* ing){return !(strcmp(n, ing->get_nume()));});
                        if(it != ingrediente.end()){
                            cin>>cant;
                            map_ing.insert(pair<Ingredient, int>(*(*it), cant));
                        }
                        else{
                            throw(nume);
                        }
                    }
                    catch(string s){
                        cout<<"Ingredientul "<<s<<" nu se afla pe lista de ingrediente. Introduceti alt ingredient.\n";
                        i--;
                    }
                }
                Cocktail test(map_ing);
                try{
                    if(test.verifica_cantitati()){
                        cout<<"Introduceti numele cockatailului: ";
                        getline(cin>>ws, nume);
                        cout<<"Introduceti volumul cocktailului (ml): ";
                        int ml;
                        cin>>ml;
                        cout<<"Introduceti pretul cocktailului: ";
                        double pret;
                        cin>>pret;
                        Cocktail* c = new Cocktail(nume, map_ing, ml, pret);
                        meniu.push_front(c);
                    }
                    else{
                        throw(1);
                    }
                }
                catch(int x){
                    cout<<"Suma procentelor ingredientelor nu este 100. Cocktailul nu va fi adaugat in meniu.\n";
                }
            }
            case 'p':
            {
                cout<<"Acestea sunt vinurile introduse in meniu:\n";
                Sticla_Vin v;
                list<Item*>::iterator it = meniu.begin();
                for(; it != meniu.end(); it++){
                    if(typeid(*(*it)) == typeid(v)){
                        cout<<(*it)->get_nume()<<endl;
                    }
                }
                cout<<"Introduceti numele vinului la care doriti sa adaugati versiunea la pahar: ";
                string nume;
                getline(cin>>ws, nume);
                try{
                    list<Item*>::iterator i = find_if(meniu.begin(), meniu.end(), [&](Item* ite){return nume == ite->get_nume();});
                    if(i != meniu.end()){
                        Sticla_Vin* s = dynamic_cast<Sticla_Vin*>(*i);
                        Pahar_Vin* pahar = new Pahar_Vin(*s);
                        pahar->calc_pret();
                        meniu.push_front(pahar);
                    }
                    else{
                        throw(nume);
                    }
                }
                catch(string s){
                    cout<<"Vinul "<<s<<" nu este in meniu. Itemul nu va fi adaugat in meniu.\n";
                }
            }
            default:
                break;
            }
        break;
        }
        case '2':
        {
            string nume;
            double pret;
            double alcoolemie;
            cout<<"Introdu caracteristicile ingredientului: \n";
            cout<<"Nume: ";
            getline(cin>>ws, nume);
            cout<<"Pret: ";
            cin>>pret;
            cout<<"Alcoolemie: ";
            cin>>alcoolemie;
            char* n = new char[100];
            size_t length = nume.copy(n, nume.size(), 0);
            n[length] = '\0';
            Ingredient* ing = new Ingredient(n, pret, alcoolemie);
            ingrediente.insert(ing);
            set<Ingredient*>::iterator i = ingrediente.begin();
            cout<<"Ati introdus in lista de ingrediente: "<<*(*i)<<endl;
            break;
        }
        case '3':
        {
            list<Item*>::iterator i = meniu.begin();
            for(; i != --(meniu.end()); i++){
                cout<<*(*i)<<", ";
            }
            cout<<*(*i);
            cout<<endl;
            break;
        }
        case '4':
        {
            set<Ingredient*>::iterator i = ingrediente.begin();
            for(; i != --(ingrediente.end()); i++){
                cout<<*(*i)<<", ";
            }
            cout<<*(*i);
            cout<<endl;
            break;
        }
        case '5':
        {
            cout<<"Ati ales sa adaugati itemuri in meniu din fisier. Fisierul va trebui sa contina: pe primul rand un numar natural n egal cu numarul de itemuri care vor fi adaugate. ";
            cout<<"Pe urmatoarele randuri vor fi introduse: o litera v sau c in functie de tipul de item care va fi adaugat (vin sau cocktail); ";
            cout<<"daca litera este v pe urmatoarele randuri vor fi: numele, pretul brut, pretul in meniu, alcoolemia, culoarea, gustul si alcoolemia vinului; ";
            cout<<"daca litera este c pe urmatoarele randuri vor fi: numarul de ingrediente m, 2*m randuri pe care vor fi numele ingredientului/procentul (ingredientele trebuie sa fie deja introduse in lista de ingrediente) in care este folosit in cocktail, apoi numele cocktailului, volumul lui (in ml) si pretul in meniu.\n";
            cout<<"Introduceti numele fisierului (cu tot cu extensia .txt): ";
            string numefisier;
            getline(cin>>ws, numefisier);
            ifstream in;
            in.open(numefisier);
            int n;
            in>>n;
            for(int i = 0; i < n; i++){
                char opt;
                in>>opt;
                if(opt == 'v'){
                    string nume;
                    double pret_brut;
                    double pret_meniu;
                    double alcoolemie;
                    string culoare;
                    string gust;
                    string soi;
                    int an;
                    getline(in>>ws, nume);
                    in>>pret_brut;
                    in>>pret_meniu;
                    in>>alcoolemie;
                    in>>culoare;
                    in>>gust;
                    in>>soi;
                    in>>an;
                    Sticla_Vin* sv = new Sticla_Vin(nume, pret_brut, pret_meniu, alcoolemie, culoare, gust, soi, an);
                    meniu.push_front(sv);
                }
                else if(opt == 'c'){
                    int nring;
                    in>>nring;
                    string nume;
                    int cant;
                    map<Ingredient, int> map_ing;
                    int ok = 1;
                    for(int j = 0; j < nring; j++){
                        getline(in>>ws, nume);
                        char* n = new char[100];
                        size_t length = nume.copy(n, nume.size(), 0); //https://www.techiedelight.com/convert-string-char-array-cpp/
                        n[length] = '\0';
                        try{
                            set<Ingredient*>::iterator it = find_if(ingrediente.begin(), ingrediente.end(), [&](Ingredient* ing){return !(strcmp(n, ing->get_nume()));});
                            if(it != ingrediente.end()){
                                in>>cant;
                                map_ing.insert(pair<Ingredient, int>(*(*it), cant));
                            }
                            else{
                                throw(nume);
                            }
                        }
                        catch(string s){
                            cout<<"Ingredientul "<<s<<" nu se afla pe lista de ingrediente. Cocktailul nu va fi adaugat in meniu.\n";
                            j = nring;
                            ok = 0;
                        }
                    }
                    if(ok){
                        getline(in>>ws, nume);
                        int ml;
                        in>>ml;
                        double pret;
                        in>>pret;
                        Cocktail* c = new Cocktail(nume, map_ing, ml, pret);
                        try{
                            if(c->verifica_cantitati()){
                                meniu.push_front(c);
                            }
                            else{
                                throw(1);
                            }
                        }
                        catch(int x){
                            cout<<"Suma procentelor ingredientelor nu este 100. Cocktailul nu va fi adaugat in meniu.\n";
                        }
                    }
                    else{
                        ;
                    }
                }
            }
            break;
        }
        case '6':
        {
            cout<<"Ati ales sa adaugati ingrediente din fisier. Fisierul va trebui sa contina: pe primul rand un numar natural n egal cu numarul de ingrediente care vor fi adaugate. Pe urmatoarele 3*n randuri vor fi numele/pretul/alcoolemia fiecarui ingredient.\n";
            cout<<"Introduceti numele fisierului (cu tot cu extensia .txt): ";
            string numefisier;
            getline(cin>>ws, numefisier);
            ifstream in;
            in.open(numefisier);
            int n;
            in>>n;
            string nume;
            double pret;
            double alcoolemie;
            for(int i = 0; i < n; i++){
                getline(in>>ws, nume);
                char* n = new char[100];
                size_t length = nume.copy(n, nume.size(), 0);
                n[length] = '\0';
                in>>pret;
                in>>alcoolemie;
                Ingredient* ing = new Ingredient(n, pret, alcoolemie);
                ingrediente.insert(ing);
            }
            break;
        }
        case '7':
        {
            ofstream out("meniu.txt");
            list<Item*>::iterator i = meniu.begin();
            for(; i != meniu.end(); i++){
                out<<*(*i)<<endl;
            }
            out.close();
            break;
        }
        case '8':
        {
            ofstream out("ingrediente.txt");
            set<Ingredient*>::iterator i = ingrediente.begin();
            for(; i != ingrediente.end(); i++){
                out<<*(*i)<<endl;
            }
            out.close();
        }
        case '9':
        {
            cout<<"Meniul este:\n";
            list<Item*>::iterator i = meniu.begin();
            for(; i != meniu.end(); i++){
                cout<<*(*i)<<endl;
            }
            cout<<"Introduceti numele unui item din meniu cu care sa incepeti comanda:";
            string nume;
            getline(cin>>ws, nume);
            try{
                list<Item*>::iterator it = find_if(meniu.begin(), meniu.end(), [&](Item* ite){return nume == ite->get_nume();});
                if(it != meniu.end()){
                    Comanda* c = new Comanda(*(*it));
                    comenzi.push_back(c);
                }
                else{
                    throw(nume);
                }
            }
            catch(string s){
                cout<<"Itemul "<<s<<" nu este in meniu. Comanda nu va fi creata";
            }
            break;
        }
        case 'a':
        {
            cout<<"Comenzile deschise sunt:\n";
            for(unsigned int i = 0; i < comenzi.size(); i++){ //am facut unsigned sa scap de un warning
                cout<<"Comanda "<<i+1<<":\n";
                cout<<*(comenzi[i]);
            }
            cout<<endl<<"Introduceti numarul comenzii la care doriti sa adaugati: ";
            int nr;
            cin>>nr;
            nr--;
            cout<<"Meniul este:\n";
            list<Item*>::iterator i = meniu.begin();
            for(; i != meniu.end(); i++){
                cout<<(*i)->get_nume()<<endl;
            }
            cout<<"Introduceti numele unui item din meniu pe care sa il adaugati la comanda selectata:";
            string nume;
            getline(cin>>ws, nume);
            try{
                list<Item*>::iterator it = find_if(meniu.begin(), meniu.end(), [&](Item* ite){return nume == ite->get_nume();});
                if(it != meniu.end()){
                    *(comenzi[nr]) += *(*it);
                }
                else{
                    throw(nume);
                }
            }
            catch(string s){
                cout<<"Itemul "<<s<<" nu este in meniu. Comanda selectata nu va fi modificata.\n";
            }
            break;
        }
        case 'b':
        {
            cout<<"Comenzile deschise sunt:\n";
            for(unsigned int i = 0; i < comenzi.size(); i++){ //am facut unsigned sa scap de un warning
                cout<<"Comanda "<<i+1<<":\n";
                cout<<*(comenzi[i]);
            }
            break;
        }
        case '0':
            break;
        default:
            break;
        }
    }

    return 0;
}
