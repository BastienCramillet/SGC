//
// Copyright (C) 2011 Xavier MICHEL (xavier.michel.mx440@gmail.com)
//
// This software is provided as-is, without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//

#ifndef LOG_MANAGER_HPP
#define LOG_MANAGER_HPP

#include <iostream>
#include <cstring>
#include <sstream>

#include <map>
#include <set>
#include <list>
#include <vector>
#include <deque>


// comment next line if you don't wanna see log
#define SHOW_LOG



/*!
    \version 0.3

    \class Log
    Gestion des logs (en fait cette classe pourrait s'appeler LogManager, mais ce serait trop long à écrire pour saisir un log)

    Si le flag SHOW_LOG est défini, les logs sont traités sinon ils sont ignorés et ne seront pas dans le binaire final.

    De la même manière que pour les logs d'Andoid, il existe différents niveaux de log :
    - VERBOSE : v(tag)
    - DEBUG   : d(tag)
    - INFO    : i(tag)
    - WARNING : w(tag)
    - ERROR   : e(tag)

    Bien sûr la description des niveaux de log n'est pas objective, elle correspond plus à ce que moi je mettrais !
    ---
    - verbose : Salut ! Je suis le logiciel et j'ai décidé de faire ça, ça et ça ! C'est bien hein ? Bon je suis le niveau le plus bas alors j'affiche plein d'info sur le déroulement du programme pas très utiles.
    - debug   : A peine au dessus du verbose, j'essaye de filtrer un peu les données utiles en affichant des données à l'utilisateur par exemple "Tiens, tu savais que j'avais trouvé n items dans le xml ?"
    - info    : Je dirais que c'est pour indiquer que l'utilisateur essaye de se servir du logiciel, genre "arf, il tente un drag... c'est pas prévu, ça va bugger ou pas ! On verra bien après tout"
    - warning : Là pour moi, c'est quand le programme balance des exceptions, il affiche un warning au passage, exemple : "Objet sélectionné incorrect, normalement c'est prévu mais attention"
    - error   : Le summum, aucune excuse, la terrible segfault à tous les coups ! Exemple : "La factory ne connais pas cet argument, soyez sûr qu'elle va renvoyer n'importe quoi et priez !"

    On peut définir le niveau désiré via void setLogLevel(LogLevel).

    On peut définir le flux de sortie via void setOutput(std::ostream &stream).

    Exemple d'utilisation rapide :

    \verbatim
    int main() {

        std::ofstream file("logs.txt");
        Log::setOutput(file);   // écrire les messages de log dans un fichier

        Log::setOutput(std::cout); // revenir sur la sortie standard

        Log::setLogLevel(DEBUG);   // niveau minium des logs à afficher

        Log::v("main") << "Welcome in my testing prog";

        bool b = false;
        Log::i("main") << "Demain il " << 9 << " fera pas beau : " << b;    // voir spécialisation template bool (log.inl)

        Foo f;
        Log::d("foo") << f; // en supposant que foo redéfinisse l'opérateur << pour un ostream

        return 0;
    }
    \endverbatim
*/


enum LogLevel {
    VERBOSE,
    DEBUG,
    INFO,
    WARNING,
    ERRORS
};


class Log {

public :

#ifndef SHOW_LOG

    class FakeDebug;

    /*!
        \brief Nouvelle ligne de log en verbose, version fake
    */
    static inline FakeDebug v(const std::string & = "") {
        return FakeDebug();
    }

    /*!
        \brief Nouvelle ligne de log en debug, version fake
    */
    static inline FakeDebug d(const std::string & = "") {
        return FakeDebug();
    }

    /*!
        \brief Nouvelle ligne de log en information, version fake
    */
    static inline FakeDebug i(const std::string & = "") {
        return FakeDebug();
    }

    /*!
        \brief Nouvelle ligne de log en warning, version fake
    */
    static inline FakeDebug w(const std::string & = "") {
        return FakeDebug();
    }

    /*!
        \brief Nouvelle ligne de log en error, version fake
    */
    static inline FakeDebug e(const std::string & = "") {
        return FakeDebug();
    }


    /*!
        \brief Défini le niveau de sortie désirés, version fake
    */
    static inline void setLogLevel(LogLevel) {
    }

    /*!
        \brief Défini le flux de sortie, version fake
    */
    static inline void setOutput(const std::ostream &) {
    }


    /*!
        \class FakeDebug
        Cette classe sera effacée par le compilateur, elle sert quand il ne faut pas traiter le débugage
    */
    class FakeDebug
    {
        public:

            /*!
                Un traitement bien inutile ...
            */
            template<class T>
            inline FakeDebug& operator<<(const T &) {
                return *this;
            }

    };


#
#else // On doit traiter les logs
#

    class LogLine;

    /*!
        \brief Nouvelle ligne de log en verbose
    */
    static inline LogLine v(const std::string &tag = "") {
        return LogLine(VERBOSE, tag);
    }

    /*!
        \brief Nouvelle ligne de log en debug
    */
    static inline LogLine d(const std::string &tag = "") {
        return LogLine(DEBUG, tag);
    }

    /*!
        \brief Nouvelle ligne de log en information
    */
    static inline LogLine i(const std::string &tag = "") {
        return LogLine(INFO, tag);
    }

    /*!
        \brief Nouvelle ligne de log en warning
    */
    static inline LogLine w(const std::string &tag = "") {
        return LogLine(WARNING, tag);
    }

    /*!
        \brief Nouvelle ligne de log en error
    */
    static inline LogLine e(const std::string &tag = "") {
        return LogLine(ERRORS, tag);
    }


    /*!
        \brief Défini le niveau de sortie désirés
    */
    static void setLogLevel(LogLevel l);

    /*!
        \brief Défini le flux de sortie
    */
    static void setOutput(std::ostream &stream);


    /*!
        \class LogLine
        Une ligne à écrire dans les logs
    */
    class LogLine {

        public:

            /*!
                \brief Construit une ligne de log de niveau l avec pour tag tag
            */
            inline LogLine(LogLevel l, const std::string &tag)
                : m_logLevel(l), m_tag(tag), m_message(new std::ostringstream()) {
            }

            /*!
                \brief A sa destruction, la ligne est envoyée au logManager pour être traitée (affichée)
            */
            inline ~LogLine() {
                Log::writeLine(m_logLevel, m_tag, m_message->str());
                delete m_message;
            }

            /*!
                \brief On veut écrire quelque chose dans les logs
                \attention Ce template est spécialisé / spécialisable dans log.inl
            */
            template<class T>
            inline LogLine& operator<<(const T &something) {
                *m_message << something;
                return addSpace();
            }

            /*!
                \brief La spécialisation pour std::pair, définie dans log.inl
            */
            template<class T1, class T2>
            inline LogLine& operator<<(const std::pair<T1, T2> &pair);

            /*!
                \brief La spécialisation pour std::map, définie dans log.inl
            */
            template<class T1, class T2>
            inline LogLine& operator<<(const std::map<T1, T2> &map);

            /*!
                \brief La spécialisation pour std::set, définie dans log.inl
            */
            template<class T>
            inline LogLine& operator<<(const std::set<T> &set);

            /*!
                \brief La spécialisation pour std::list, définie dans log.inl
            */
            template<class T>
            inline LogLine& operator<<(const std::list<T> &list);

            /*!
                \brief La spécialisation pour std::vector, définie dans log.inl
            */
            template<class T>
            inline LogLine& operator<<(const std::vector<T> &vector);

            /*!
                \brief La spécialisation pour std::deque, définie dans log.inl
            */
            template<class T>
            inline LogLine& operator<<(const std::deque<T> &deque);

            /*!
                \brief La spécialisation pour std::multimap, définie dans log.inl
            */
            template<class T1, class T2>
            inline LogLine& operator<<(const std::multimap<T1, T2> &multimap);

            /*!
                \brief La spécialisation pour std::multiset, définie dans log.inl
            */
            template<class T>
            inline LogLine& operator<<(const std::multiset<T> &multiset);


         private :

             /*!
                \brief Ajoute un espace en fin de ligne
             */
             inline LogLine& addSpace() {
                 *m_message << " ";
                 return *this;
             }


             LogLevel m_logLevel;               //!< Le niveau de log de cette ligne

             std::string m_tag;                 //!< Le tag associé à cette ligne

             std::ostringstream *m_message;     //!< Le flux dans lequel on écrit la ligne

     };



private :

    Log();

    /*!
        \brief écris une ligne dans le log
    */
    static void writeLine(
            LogLevel level,
            const std::string &tag,
            const std::string &message
         );



    static Log m_log;                               //!< instance unique de Log (LogManager)

    std::map<LogLevel, std::string> m_logLabels;    //!< Les niveaux de log avec le message correspondant

    LogLevel m_logLevel;                            //!< Le niveau de log actuel

    std::ostream *m_ostream;                        //!< Le flux de sortie actuel


#endif // SHOW_LOG


};  // fin de la classe


#ifdef SHOW_LOG
#include "Log.inl"  // spécialisation template
#endif // SHOW_LOG


#endif // LOG_MANAGER_HPP
