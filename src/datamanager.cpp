#include "constants.hpp"
#include "datamanager.hpp"
#include "engine.hpp"

#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;
using namespace pugi;

//
// Sauvegarde les données de la partie
// Le nom du fichier est donné dans les constantes
//
void DataManager::save()
{
    Engine* engine = Engine::getInstance();

    stringstream sstr;

    sstr << "<?xml version=\"1.0\"?>" << std::endl;
    sstr << "<gamesave name=\"current\">" << std::endl;

    sstr << engine->getDataXml() << endl;
    sstr << engine->getPlayer().getDataXml() << endl;
    sstr << engine->getEntityManager().getDataXml() << endl;
    sstr << engine->getMap().getDataXml() << endl;

    sstr << "</gamesave>" << std::endl;

    //save dans le fichier
    xml_document doc;
    xml_parse_result result = doc.load(sstr);

    if (result)
        cout << ">> Parsing des données avant sauvegarde ok." << endl;
    else
        cout << ">> Problème lors du parsing des données avant sauvegarde." << endl;

    ofstream ofile(SAVE_FILEPATH, ios::out | ios::trunc);

    if (ofile)
    {
        ofile << sstr.str();
        ofile.close();
    }
}

//
// Chargement de la partie
//
bool DataManager::load()
{
    ifstream file(SAVE_FILEPATH, ios::in);

    if (!file)
    {
        cout << "[DataManager::DataManager::load] Erreur lors du chargement du fichier de sauvegarde : [" << SAVE_FILEPATH << "]" << endl;
    }

    xml_parse_result result = xmlDocument.load(file);

    if (result)
    {
        cout << "[DataManager::load] xmlDocument chargé avec succès" << endl;
        return true;
    }
    else
    {
        cout << "[DataManager::load] xmlDocument [" << SAVE_FILEPATH << "] contient des erreurs, attr value: [" << xmlDocument.child("node").attribute("attr").value() << "]" << endl;
        cout << "Error description: " << result.description() << endl;
        cout << "Error offset: " << result.offset << " (error at [..." << SAVE_FILEPATH << result.offset << "]" << endl;
        return false;
    }
}

//
// Supprime le fichier de sauvegarde
//  SAVE_FILEPATH
//
bool DataManager::deleteSaveFile()
{
    return (remove(SAVE_FILEPATH.c_str()) == 0);
}

//
// Récupère la valeur contenu dans un tag unique <ptag>
//
int DataManager::getIntValueFromTag(const string& ptag)
{
    xml_node node = xmlDocument.child("gamesave").child(ptag.c_str());

    if (!node)
        return -1;

    if (node.child_value() == "")
        return -1;

    return stoi(node.child_value());
}

//
// Retourne le node complet pnodeName
//
xml_node DataManager::getNode(const string& pnodePath)
{
    return xmlDocument.first_element_by_path(pnodePath.c_str());
}
