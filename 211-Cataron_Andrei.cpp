/* Cataron Andrei-Vlad grupa 211
CODEBLOCKS
Tutore laborator: Eduard Szmeteanca*/

#include <iostream>
#include <string>
#include <set>
#include <vector>

using namespace std;

class Vaccin{
    double pret;
    int temperatura_depozitare;
    set<string> substante_in_compozitie;
    string nume_producator;
public:
    Vaccin(){}
    ~Vaccin(){}
    Vaccin(double, int, set<string>, string);
    Vaccin(const Vaccin&);
    Vaccin& operator=(Vaccin&);

    virtual ostream& print(ostream&) const; ///afisare luata din proiect
    friend ostream& operator<<(ostream&, const Vaccin&);
    //friend istream& operator>>(istream&, Vaccin&);

    virtual void afisareSchemaVaccinare() = 0;

    double get_pret(){return pret;}
    int get_temperatura(){return temperatura_depozitare;}
    string get_producator(){return nume_producator;}
    void set_pret(double p){this->pret = p;}
    void set_temp(int t){this->temperatura_depozitare = t;}
    void set_substante(set<string> s){this->substante_in_compozitie = s;}
    void set_nume(string s){this->nume_producator = s;}
    set<string> get_substante(){return substante_in_compozitie;}
};

class VaccinAntigripal: public Vaccin{
    set<string> tulpini;
    bool recomandariOMS;
public:
    VaccinAntigripal(){}
    ~VaccinAntigripal(){}
    VaccinAntigripal(double, int, set<string>, string, set<string>, bool);

    ostream& print(ostream&) const;
    friend istream& operator>>(istream&, VaccinAntigripal&);

    void afisareSchemaVaccinare();
};

class VaccinAntihepatic: public Vaccin{
    char tip_hepatita;
    string mod_vaccinare;
public:
    VaccinAntihepatic(){}
    ~VaccinAntihepatic(){}
    VaccinAntihepatic(double, int, set<string>, string, char, string);

    ostream& print(ostream&) const;
    friend istream& operator>>(istream&, VaccinAntihepatic&);

    void afisareSchemaVaccinare();
};

class VaccinAntiSarsCov2: public Vaccin{
    set<string> reactii_adverse;
    double rata_eficienta;
    set<string> medicamente_contraindicate;
public:
    VaccinAntiSarsCov2(){}
    ~VaccinAntiSarsCov2(){}
    VaccinAntiSarsCov2(double, int, set<string>, string, set<string>, double, set<string>);

    ostream& print(ostream&) const;
    friend istream& operator>>(istream&, VaccinAntiSarsCov2&);

    void afisareSchemaVaccinare();

};

class Comanda{
    int id;
    string data;
    string nume_client;
    string tip;
public:
    Comanda(){}
    ~Comanda(){}
    Comanda( string data, string nume_client, string tip, int id){
        this->id = id;
        this->data = data;
        this->nume_client = nume_client;
        this->tip = tip;
    }

    friend ostream& operator<<(ostream& out, Comanda& com){
        out<<com.data<<" "<<com.nume_client<<" "<<com.tip<<endl;
        return out;
    }

};

class ListaComenzi{
    vector<Comanda*> v;
public:
    ListaComenzi(){}
    ~ListaComenzi(){}

    void adauga_vaccin(Comanda* vac){
        this->v.push_back(vac);
    }

    ListaComenzi& operator+(Comanda& c){
        this->v.push_back(&c);
        return *this;
    }

    Comanda* operator[](int i){
        return this->v[i];
    }
};


///Vaccin
Vaccin::Vaccin(double pret, int temperatura_depozitare, set<string> substante_in_compozitie, string nume_producator){
    this->pret = pret;
    this->temperatura_depozitare = temperatura_depozitare;
    this->substante_in_compozitie = substante_in_compozitie;
    this->nume_producator = nume_producator;
}

Vaccin::Vaccin(const Vaccin& ob){
    this->pret = ob.pret;
    this->temperatura_depozitare = ob.temperatura_depozitare;
    this->substante_in_compozitie = ob.substante_in_compozitie;
    this->nume_producator = ob.nume_producator;
}

Vaccin& Vaccin::operator=(Vaccin& ob){
    this->pret = ob.pret;
    this->temperatura_depozitare = ob.temperatura_depozitare;
    this->substante_in_compozitie = ob.substante_in_compozitie;
    this->nume_producator = ob.nume_producator;
    return *this;
}

ostream& operator<<(ostream& out, const Vaccin& ob){
	ob.print(out);
	return out;
}

ostream& Vaccin::print(ostream& out) const{
    out<<"pret: "<<this->pret<<"; tempretura depozitare: "<<this->temperatura_depozitare<<"; substante in compozitie: ";
    set<string>::iterator it = this->substante_in_compozitie.begin();
    for(; it != --(this->substante_in_compozitie.end()); it++){
        out<<*it<<", ";
    }
    out<<*it;
    return out;
}


///VaccinAntigripal

VaccinAntigripal::VaccinAntigripal(double p, int t, set<string> s, string n, set<string> tulpini, bool recomandariOMS):Vaccin(p, t, s, n){
    this->tulpini = tulpini;
    this->recomandariOMS = recomandariOMS;
}

ostream& VaccinAntigripal::print(ostream& out) const{
    out<<"Vaccin Antigripal";
    return out;
}

void VaccinAntigripal::afisareSchemaVaccinare(){
    cout<<"Vaccinul se administreaza la adulti o doza de 0.5 ml, iar la copii si adolescenti o doza de 0.3 ml, repetandu-se din 2 in 2 ani.\n";
}

istream& operator>>(istream& in, VaccinAntigripal& v){
    cout<<"Citire antigripal\n";
    cout<<"pret: ";
    double p;
    in>>p;
    v.set_pret(p);
    cout<<"temperatura: ";
    int t;
    in>>t;
    v.set_temp(t);
    cout<<"numar substante in compozitie: ";
    int n;
    in>>n;
    cout<<"introducti substantele: ";
    string s;
    set<string> subst;
    for(int i = 0; i < n; i++){
        in>>s;
        subst.insert(s);
    }
    v.set_substante(subst);
    cout<<"recomandare OMS(0- nu, 1-da): ";
    in>>v.recomandariOMS;
    return in;
}

///VaccinAntihepatic

VaccinAntihepatic::VaccinAntihepatic(double p, int t, set<string> s, string n, char tip_hepatita, string mod_vaccinare):Vaccin(p, t, s, n){
    this->tip_hepatita = tip_hepatita;
    this->mod_vaccinare = mod_vaccinare;
}

ostream& VaccinAntihepatic::print(ostream& out) const{
    out<<"Vaccin Antihepatic pentru hepatita "<<this->tip_hepatita<<" care se administreaza "<<this->mod_vaccinare;
    out<<". Alte detalii: ";
    Vaccin::print(out);
    out<<".\n";
    return out;
}

void VaccinAntihepatic::afisareSchemaVaccinare(){
    cout<<"Vaccinul se administreaza astfel: cel Antihepatita A si B, la copii cu varsta mai mica de 1 an se adminstreaza 2 injectari la un interval de o luna, a treia injectare după 6 luni de la prima administrare, la adulti conform schemei de imunizare recomandata de medic; cel Antihepatita C doar la recomandarea medicului.\n";
}

istream& operator>>(istream& in, VaccinAntihepatic& v){
    cout<<"Citire antihepatic\n";
    return in;
}

///VaccinCovid

VaccinAntiSarsCov2::VaccinAntiSarsCov2(double p, int t, set<string> s, string n, set<string> reactii_adverse, double rata_eficienta, set<string> medicamente_contraindicate):Vaccin(p, t, s, n){
    this->reactii_adverse = reactii_adverse;
    this->rata_eficienta = rata_eficienta;
    this->medicamente_contraindicate = medicamente_contraindicate;
}

ostream& VaccinAntiSarsCov2::print(ostream& out) const{
    out<<"Vaccin Anti Covid. Detalii: ";
    Vaccin::print(out);
    out<<"; reactii adverse posibile: ";
    set<string>::iterator it = this->reactii_adverse.begin();
    for(; it != --(this->reactii_adverse.end()); it++){
        out<<*it<<", ";
    }
    out<<*it;
    out<<"; rata eficienta: "<<this->rata_eficienta<<"; medicamente constraindicate: ";
    it = this->medicamente_contraindicate.begin();
    for(; it != --(this->medicamente_contraindicate.end()); it++){
        out<<*it<<", ";
    }
    out<<*it;
    out<<".\n";
    return out;
}

void VaccinAntiSarsCov2::afisareSchemaVaccinare(){
    cout<<"Vaccinul anti covid se administreaza persoanelor cu varsta de peste 16 ani, 2 doze la o perioada de 21 zile.\n";
}

istream& operator>>(istream& in, VaccinAntiSarsCov2& v){
    cout<<"Citire covid\n";
    return in;
}

class Meniu{///sursa: exemple singleton curs
    static Meniu* instanta;
    Meniu(){}
    vector<Vaccin*> v;
public:
    static Meniu* get_instanta(){
        if(instanta == NULL){
            instanta = new Meniu;
        }
        return instanta;
    }
    void afisare_vaccinuri(){
        for(unsigned int i = 0; i < v.size(); i++){
            cout<<*v[i]<<endl;
        }
    }
    void afisare_producatori(){
        for(unsigned int i = 0; i < v.size(); i++){
            cout<<v[i]->get_producator();
        }
    }
};
Meniu* Meniu::instanta = NULL;

int main()
{
    VaccinAntigripal a;
    VaccinAntihepatic b;
    VaccinAntiSarsCov2 c;
    cin>>a>>b>>c;///nu am avut timp sa scriu codul de citire pentru fiecare vaccin
    //cout<<a<<b<<c;///din aceasta cauza sunt probleme si la afisare deoarece afisarile sunt facute complet si se incearca sa se afiseze lucrui neinitializate
    Vaccin* list1[3];
    list1[0]=new VaccinAntigripal();
    list1[1]=new VaccinAntihepatic();
    list1[2]=new VaccinAntiSarsCov2();
    for(int i=0;i<3;i++)
        list1[i]->afisareSchemaVaccinare();
    Comanda com1("20-12-2020", "SpitalX", "AntiSarsCov2",20);
    ListaComenzi lista;
    lista=lista+com1;
    cout<<*(lista[0]);
    return 0;
}
