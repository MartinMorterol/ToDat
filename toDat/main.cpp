#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <sstream>
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

void analyzeReaction(string reaction, string separateur,    set<string>& setEnzime ,    set<string>& meta ,     vector<string>& cat )
{
    auto pos = reaction.find(separateur);
    string debut = reaction.substr(0,pos);
    debut = trim(debut);

    string fin = reaction.substr(pos+separateur.size());
    fin = trim(fin);

    auto vec_nom = decoupe(debut," + ") ;
    removeElem(vec_nom,"+");
    size_t    cpt =0;
    for ( auto s : vec_nom)
    {

        s = replaceAll(s ," ","_");
        s = fixStochio(s);
        cat.back() += s;
         cpt++;
        if ( cpt != vec_nom.size() )
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
    cpt=0;
    for ( auto s : vec_nom)
    {
        s = replaceAll(s ," ","_");
        s = fixStochio(s);
        cat.back() += s;
         cpt++;
        if ( cpt != vec_nom.size() )
        {
             cat.back() += " + ";
        }
       // cout << s << endl;

        auto nom_final = decoupe(s," ");
        meta.insert(nom_final.back());
    }
    auto temp = decoupe(fin," + ") ;
}
int main(int argc,char** argv)
{
    string nom  ;
    if ( argc < 2 )
    {
        cout << "Il faut au moins un argument "<<endl;;
        cout.flush();
        nom = "C:/Users/Martin/Desktop/catabolique_simple.txt";

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

            if ( isReversible(deuxieme_partie) )
            {
                analyzeReaction(deuxieme_partie,"<->",enzimeR,meta,cat);
                enzimeR.insert(nomEnzime);
            }
            else
            {
                analyzeReaction(deuxieme_partie,"->",enzimeI,meta,cat);
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
            cout << s << endl;
        }




    }
    else
    {
        cout << "erreur ouverture fichier :/ "<< endl;
    }


}
