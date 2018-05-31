#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <string>
#include "pugixml.hpp"

class DataManager
{
public:
    DataManager() {}
    ~DataManager() {}

    void save();                            				//sauvegarde les données dans un fichier xml
    bool load();                            				//charge les données de la partie depuis le fichier xml

    bool deleteSaveFile();                      			//supprime le fichier de sauvegarde courant

    int getIntValueFromTag(const std::string&);             //récupère la valeur depuis le tag xml donné
    pugi::xml_node getNode(const std::string&);             //récupère le node

private:

    pugi::xml_document xmlDocument;                 		//document XML contenant les données du fichier xml
};

#endif //DATAMANAGER_HPP