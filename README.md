# 67 million years BC

# Short description 
This project is not a standalone. It requires a pool of scripts that was given by the teachers during the course "3D computer graphics" at Ecole polytechnique. For copyright reasons, this code was not made available here. This project was done alone and all my personnal code is available here.

The course aimed to give basis of OPENGL and C++ to students. It lasted for 10 weeks, with the last two to three weeks saved for the project. 

The goal was to create a small 3D scene of our choice that used what we have learned while being different.

My goal was to create a planet and add two species of dinosaurs, a herbivor one called Chilesaurus and a carnivor one called Tyrannosaurus that would chase the first one. I also added group behaviors to both species.

You can find a picture which illustrates the architecture of the project and classes links along with a short PDF file which describes, in french, what was done. A short video of the project is also available. It was recorded using OBS.

## Author 

[Paul Calot](https://www.linkedin.com/in/paul-calot-43549814b/)

## Appendix 

### Pour réutilisation dans le projet complet (FR)

Le dossier principal doit être nommé "projet". Je l'avais placé dans /inf443_vcl-master/scenes

Les headers current_scene.hpp et scenes.hpp sont également disponibles.

Le projet en lui même est organisé en les classes suivantes : 

	- main_projet
	- Planet et TerrainFace (pour la création de la planète, comme un cube à 6 TerrainFace d'abord)
	- Dinosaur
		- Chilosaurus (la proie ici)
 		- Tyrannosaurus (le prédateur)
	- DinosaurHierarchy (création du mesh_drawable et animation)
	- Meteorite
	- Grid (grille 3D - une grille 2D par face - pour stocker les dinosaures - utile pour le comportement de groupes et le recherche des voisins)

Ajouté à cela est une script Utility avec quelques unes des fonctions réutilisée plusieurs fois ainsi qu'un dossier contenant les textures utilisées.

le fichier "architecture.png" montre les relations entre ces classes.
	
Vous trouverez également une courte vidéo du projet intitulée "65M_years_BC.mp4".