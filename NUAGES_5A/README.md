STEPS:
- Prendre le spectre 0 - 255 et le diviser en 8 classes
- Trouver le centre de chaque classe et l'assigner
- Pour chaque pixel p:
  -> Calculer le vecteur Vp correspondant à la couleur du pixel et de ses voisins directs (qui le touche)
  -> Trouver la classe dont le centre de gravite est le plus proche de son centre de gravite
  -> l'assigner a cette classe
- A la fin de l'iter, calculer les nouveaux centre de gravite en prenant la mediane des centres de gravite de tout les pixels appartenant a la classes
- Recommencer tant que les centres des classes bougent
- Mettre les pixels appartenant a la classe la plus haute (celles des nuages) en blanc et les autres en noirs

/!\ POUR LES PIXELS AU BORD, mettre -1 en valeur des voisins et ne pas les prendre
en compte lors du calcul de la mediane


STRUCTS:
struct pixel {
  struct* classe cl;
  int *v;
}
