#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <vector>
#include <sstream>
#include <iostream>
#include <iterator>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

/// copier coller google :D
// trim from start
static inline std::string& ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string& rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string& trim(std::string &s) {
        return ltrim(rtrim(s));
}

using namespace std;
struct CleeValeur
{
    CleeValeur(string uneClee, string uneValeur) : clee(uneClee),valeur(uneValeur) {

        std::size_t found = valeur.find('.');
        if (found!=std::string::npos)
            valeur[found] = ',';
    }
    string clee;
    string valeur;
};

struct FichierValeur
{
    string nom;
    vector<CleeValeur> vecCleeValeur;
    FichierValeur(string unNom) : nom(unNom) {}
    void add (CleeValeur cv) {vecCleeValeur.push_back(cv); }
};

struct FutureCSV
{
    string nom;
    vector<FichierValeur> vecFichierValeur;
    FutureCSV(string unNom) : nom(unNom) {}
    set<int> setClee;
    void nouveauFichier(string unNom) {vecFichierValeur.push_back({unNom}); };
    void addFichierCourant(CleeValeur cv ){
        vecFichierValeur.back().add(cv);
        // ça marche car les indice sont pas a virgule
        setClee.insert(std::stoi(cv.clee));
        }

    void toCSV()
    {
        std::ofstream file( nom+".csv", std::ios_base::app );
        // on met tout sous une map parce que les map, c'est simple :)
        map<int,vector<string>> laMap;
        for ( auto clee : setClee )
        {
            laMap.insert({clee,{}});
        }
        // on remplit :)
        vector<string> entete{""};

        for (auto& fv : vecFichierValeur )
        {
            // je veux que toutes les valeurs de la map soit a "" si vide
            for ( auto clee : setClee )
            {
                laMap[clee].push_back("");
            }

            entete.push_back(fv.nom);

            // on met vraiment les valeur
            std::cout << fv.vecCleeValeur.size()<<endl;;
            for (auto cv : fv.vecCleeValeur )
            {

                laMap[std::stoi(cv.clee)].back()=cv.valeur;
            }
        }

        // affichage
        for (  auto v : entete )
        {
            file << v << ";";
        }
        file << "\n";
        for ( auto pairIntVec : laMap )
        {

           file <<  pairIntVec.first << ";";
           for ( auto unString : pairIntVec.second )
           {
               file <<  unString << ";";
           }
           file << "\n";
        }
    }
};

 vector<string> liste()
{
   DIR * repertoire = opendir(".");
   vector<string> retour;
   if ( repertoire == NULL)
   {
      cout << "Impossible de lister le répertoire" << endl;
   }
   else
   {
      struct dirent * ent;

      while ( (ent = readdir(repertoire)) != NULL)
      {

             retour.push_back(ent->d_name);

      }
      closedir(repertoire);
   }
    return retour;
}
vector<string> listeClean(string& nomFichier)
{
    nomFichier = string(nomFichier.begin()+2,nomFichier.end());
    auto vec = liste();
    vector<string> retour;
    for (auto unFichier : vec )
    {
         if ( unFichier != "." && unFichier != ".." && unFichier != nomFichier )
            retour.push_back(unFichier);
    }
    return retour;
}

bool isReversible (string& deuxiemePartieDeLigne )
{
    size_t pos = deuxiemePartieDeLigne.find("->");
    if ( pos !=  string::npos )
    {
        if ( deuxiemePartieDeLigne[pos-1]=='<' )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        cout << "fichier mal formé "<< endl;
        exit(5);
    }
}

string replaceAll(string entre, string aChercher, string aMettre)
{
    size_t pos = entre.find(aChercher);
    while (pos !=string::npos )
    {
        entre.replace(pos,aChercher.size(),aMettre);
        pos = entre.find(aChercher);
    }
    return entre;
}

vector<string> decoupe (string entre, string limite)
{
    vector<string> retour;

    size_t pos = entre.find(limite);
    while (pos !=string::npos )
    {
        string temps =  entre.substr(0,pos);
        retour.push_back(trim(temps));
        entre = entre.substr(pos+limite.size());
        pos = entre.find(limite);
    }
    retour.push_back(entre);
    return retour;
}

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

string fixStochio (string entre)
{
    auto vec = decoupe(entre,"_");
    string retour ;
    for ( auto s : vec )
    {
        if ( is_number(s) )
        {
            retour += s+" ";
        }
        else
        {
            if ( s != vec.back() )
            {
                retour += s+"_";
            }
            else
            {
                retour += s;
            }
        }
    }
    return retour;
}

void removeElem (vector<string>& entre, string elem)
{
    auto it = find(entre.begin(),entre.end(),elem) ;
    while (it!= entre.end() )
    {
        entre.erase(it);
        it = find(entre.begin(),entre.end(),elem) ;
    }
}

bool isExt (string meta)
{
    if (meta.size() <=  3)
        return false;

    if (meta.substr(meta.size()-3) == "ext")
        return true;
    return false;
}
int main(int argc,char** argv)
{
    string nom  ;
    cout << "DANGER : il faut que les + soit bien entouré d'espace pour pas etre confondu avec (+)" << endl;
    cout << "DANGER : notation en 1/2 " << endl;
    if ( argc < 2 )
    {
        cout << "Il faut au moins un argument "<<endl;;
        cout.flush();
        nom = "/users/lahdak/morterol/Bureau/catabolique_simple.txt";

    }
    else
        nom = string(argv[1]);
    // le constructeur de ifstream permet d'ouvrir un fichier en lecture

    string unFichier(nom);
    set<string> enzimeR;
    set<string> enzimeI;
    set<string> meta;
    vector<string> cat;

    std::ifstream fichier( unFichier );
    if ( fichier ) // ce test échoue si le fichier n'est pas ouvert
    {

        std::string ligne; // variable contenant chaque ligne lue
        // cette boucle s'arrête dès qu'une erreur de lecture survient
        while ( std::getline( fichier, ligne ) )
        {
            auto position = ligne.find(":");
            string temp = ligne.substr(0,position);
            string nomEnzime = trim(temp);
            nomEnzime = replaceAll(nomEnzime ," ","_");
            cat.push_back(string(nomEnzime+ " : "));
            string deuxieme_partie = ligne.substr(position+1);
            /// en fait je pense qu'il y apas besoin
            //deuxieme_partie = replaceAll(deuxieme_partie,",",".");

            if ( isReversible(deuxieme_partie) )
            {
                auto pos = deuxieme_partie.find("<->");
                string debut = deuxieme_partie.substr(0,pos);
                debut = trim(debut);


                string fin = deuxieme_partie.substr(pos+3);
                fin = trim(fin);



                auto vec_nom = decoupe(debut," + ") ;
                removeElem(vec_nom,"+");
                for ( auto s : vec_nom)
                {
                    s = replaceAll(s ," ","_");
                    s = fixStochio(s);
                    cat.back() += s;
                    if ( s != vec_nom.back() )
                    {
                         cat.back() += " + ";
                    }
                    else
                    {
                        cat.back() += " = ";
                    }
                    //cout << s << endl;

                    auto nom_final = decoupe(s," ");
                    meta.insert(nom_final.back());
                }

                vec_nom = decoupe(fin," + ") ;
                removeElem(vec_nom,"+");
                for ( auto s : vec_nom)
                {
                    s = replaceAll(s ," ","_");
                    s = fixStochio(s);
                    cat.back() += s;
                    if ( s != vec_nom.back() )
                    {
                         cat.back() += " + ";
                    }
                   // cout << s << endl;

                    auto nom_final = decoupe(s," ");
                    meta.insert(nom_final.back());
                }

                auto temp = decoupe(fin," + ") ;


                //cout << "__" << endl;

                enzimeR.insert(nomEnzime);
            }
            else
            {
                 auto pos = deuxieme_partie.find("->");
                string debut = deuxieme_partie.substr(0,pos);
                debut = trim(debut);


                string fin = deuxieme_partie.substr(pos+2);
                fin = trim(fin);



                auto vec_nom = decoupe(debut," + ") ;
                removeElem(vec_nom,"+");
                for ( auto s : vec_nom)
                {
                    s = replaceAll(s ," ","_");
                    s = fixStochio(s);
                    cat.back() += s;
                    if ( s != vec_nom.back() )
                    {
                         cat.back() += " + ";
                    }
                    else
                    {
                        cat.back() += " = ";
                    }
                    //cout << s << endl;

                    auto nom_final = decoupe(s," ");
                    meta.insert(nom_final.back());
                }

                vec_nom = decoupe(fin," + ") ;
                removeElem(vec_nom,"+");
                for ( auto s : vec_nom)
                {
                    s = replaceAll(s ," ","_");
                    s = fixStochio(s);
                    cat.back() += s;
                    if ( s != vec_nom.back() )
                    {
                         cat.back() += " + ";
                    }
                   // cout << s << endl;

                    auto nom_final = decoupe(s," ");
                    meta.insert(nom_final.back());
                }

                auto temp = decoupe(fin," + ") ;


                //cout << "__" << endl;

                enzimeI.insert(nomEnzime);
            }

        }


        vector<string> metI,metE;
        for (auto s : meta )
        {
          if (isExt(s))
            metE.push_back(s);
          else
            metI.push_back(s);
        }

        cout << "-ENZREV"<< endl;
        for ( auto s :enzimeR)
             cout << s << " " ;

        cout << endl;
        cout << "-ENZIRREV"<< endl;
        for ( auto s :enzimeI)
               cout << s << " " ;

        cout << endl;

        cout << "-METINT"<< endl;
        for ( auto s :metI)
            cout << s << " " ;

        cout << endl;
        cout << "-METEXT"<< endl;
        for ( auto s :metE)
                cout << s << " " ;

        cout << endl;
        cout << "-CAT"<< endl;
        for (auto s : cat )
        {
            if (s.substr(s.size()-3) == " + ")
            {
                s = s.substr(0,s.size()-3);
            }
            cout << s << endl;
        }




    }
    else
    {
        cout << "erreur ouverture fichier :/ "<< endl;
    }


}
