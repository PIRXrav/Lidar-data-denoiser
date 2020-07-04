# Filtrage numérique des données du lidar Pepperl+Fuchs R2000. 
_Version de démonstration._

Ce programme a été utilisé lors de la coupe de France de robotique en 2019 (Équipe RCVA).  

# Démonstration

![Test Image 1](/doc/cap1.PNG)
![Test Image 2](/doc/cap2.PNG)

Signal original (bleu), signal filtré (orange).

# Explication 

### Motivations  
Enlever le bruit des scans de lidar.  

### Problème  
Comment supprimer le bruit se trouvant dans les hautes fréquences spatiales du scan tout en conservant les discontinuités de celui-ci. En effet, avec un simple filtrage passé bas nous observons le phénomène de Gibbs au niveau des discontinuités. 

### Solution  
1) La première étape est de décomposer le scan d=f(thêta) en somme de deux signaux. Le premier (AC) correspond au signal original auquel nous avons supprimé les discontinuités avec un seuil. Le second (DC) était la différence des deux derniers, il se définit alors comme une somme de porte représentant une sorte de valeur moyenne.  
2) Nous pouvons alors appliquer un filtre numérique passe bas (H) sur AC. Celui-ci doit être dimensionné soigneusement afin de conserver l'information du signal tout en filtrant suffisamment.  
3) Finalement, la reconstruction revient à la conposition des signaux initiaux avec la partie AC filtrée tel que d'= H(AC) + DC.

### Résultat  
L'algorithme est fonctionnel [HL/HL_filter/HL_Filter.cpp](../master/HL/HL_filter/HL_filter.cpp).
Aucune preuve.