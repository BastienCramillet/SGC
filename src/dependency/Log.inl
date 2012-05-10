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



// spécialisation template << sur ajout de ligne, ainsi vous pouvez vous faire plaisir et ajouter vos types =)
// bien sûr si vous modifiez ce fichier, en accord avec la licence, sachez que vos changements m'intéresse !


// spécialisation pour booléen
template<>
inline Log::LogLine& Log::LogLine::operator<< <bool>(const bool &b) {
    *m_message << (b ? "true" : "false");
    return addSpace();
}


// spécialisation pour std::string
template<>
inline Log::LogLine& Log::LogLine::operator<< <std::string>(const std::string &s) {
    *m_message << '"' << s << '"';
    return addSpace();
}


// spécialisation pour std::pair
template<class T1, class T2>
inline Log::LogLine& Log::LogLine::operator<<(const std::pair<T1, T2> &pair) {
    *this << "pair (" << pair.first << "," << pair.second << ")";
    return addSpace();
}


// spécialisation pour std::map
template<class T1, class T2>
inline Log::LogLine& Log::LogLine::operator<<(const std::map<T1, T2> &map) {
    *m_message << "map [ ";
    bool first = true;
    for (typename std::map<T1, T2>::const_iterator it = map.begin(); it != map.end(); ++it) {
        if ( ! first ) {
            *m_message << ", ";
        } else {
            first = false;
        }
        *this << "(" << it->first << "," << it->second << ")";
    }
    *m_message << "]";
    return addSpace();
}


// spécialisation pour std::set
template<class T>
inline Log::LogLine& Log::LogLine::operator<<(const std::set<T> &set) {
    *m_message << "set { ";
    bool first = true;
    for (typename std::set<T>::const_iterator it = set.begin(); it != set.end(); ++it) {
        if ( ! first ) {
            *m_message << ", ";
        } else {
            first = false;
        }
        *this << *it;
    }
    *m_message << "}";
    return addSpace();
}


// spécialisation pour std::list
template<class T>
inline Log::LogLine& Log::LogLine::operator<<(const std::list<T> &list) {
    *m_message << "list [ ";
    bool first = true;
    for (typename std::list<T>::const_iterator it = list.begin(); it != list.end(); ++it) {
        if ( ! first ) {
            *m_message << ", ";
        } else {
            first = false;
        }
        *this << *it;
    }
    *m_message << "]";
    return addSpace();
}


// spécialisation pour std::vector
template<class T>
inline Log::LogLine& Log::LogLine::operator<<(const std::vector<T> &vector) {
    *m_message << "vector [ ";
    bool first = true;
    for (typename std::vector<T>::const_iterator it = vector.begin(); it != vector.end(); ++it) {
        if ( ! first ) {
            *m_message << ", ";
        } else {
            first = false;
        }
        *this << *it;
    }
    *m_message << "]";
    return addSpace();
}


// spécialisation pour std::deque
template<class T>
inline Log::LogLine& Log::LogLine::operator<<(const std::deque<T> &deque) {
    *m_message << "deque [ ";
    bool first = true;
    for (typename std::deque<T>::const_iterator it = deque.begin(); it != deque.end(); ++it) {
        if ( ! first ) {
            *m_message << ", ";
        } else {
            first = false;
        }
        *this << *it;
    }
    *m_message << "]";
    return addSpace();
}


// spécialisation pour std::multimap
template<class T1, class T2>
inline Log::LogLine& Log::LogLine::operator<<(const std::multimap<T1, T2> &multimap) {
    *m_message << "multimap [ ";
    bool first = true;
    for (typename std::multimap<T1, T2>::const_iterator it = multimap.begin(); it != multimap.end(); ++it) {
        if ( ! first ) {
            *m_message << ", ";
        } else {
            first = false;
        }
        *this << "(" << it->first << "," << it->second << ")";
    }
    *m_message << "]";
    return addSpace();
}


// spécialisation pour std::multiset
template<class T>
inline Log::LogLine& Log::LogLine::operator<<(const std::multiset<T> &multiset) {
    *m_message << "multiset { ";
    bool first = true;
    for (typename std::multiset<T>::const_iterator it = multiset.begin(); it != multiset.end(); ++it) {
        if ( ! first ) {
            *m_message << ", ";
        } else {
            first = false;
        }
        *this << *it;
    }
    *m_message << "}";
    return addSpace();
}




// spécialisations spéciales Qt, à voir à l'usage ...
//#ifdef QT_VERSION

/* QtCore */

#include <QString>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QSize>

// QString
template<>
inline Log::LogLine& Log::LogLine::operator<< <QString>(const QString &s) {
    *m_message << '"' << s.toStdString() << '"';
    return addSpace();
}

// QDate
template<>
inline Log::LogLine& Log::LogLine::operator<< <QDate>(const QDate &d) {
    *m_message << d.toString().toStdString();
    return addSpace();
}

// QTime
template<>
inline Log::LogLine& Log::LogLine::operator<< <QTime>(const QTime &t) {
    *m_message << t.toString().toStdString();
    return addSpace();
}

// QDateTime
template<>
inline Log::LogLine& Log::LogLine::operator<< <QDateTime>(const QDateTime &dt) {
    *m_message << dt.toString().toStdString();
    return addSpace();
}

// QSize
template<>
inline Log::LogLine& Log::LogLine::operator<< <QSize>(const QSize &s) {
    *m_message << s.width() << "x" << s.height();
    return addSpace();
}


/* QtSql */

/*
#include <QSqlError>

// QSqlError
template<>
inline Log::LogLine& Log::LogLine::operator<< <QSqlError>(const QSqlError &e) {
    *m_message << e.text().toStdString();
    return addSpace();
}
*/

//#endif
