Compilation du programme :

Quoi qu'il arrive vous devez avoir téléchargé openFrameworks. Vous devez avoir sur votre machine un dossier "of_vX.X.X_vs_release". Sans ça vous n'aurez pas accès aux nombreuses dépendances nécessaires au bon fonctionnement du projet. Vous avez ensuite plusieurs possibilités pour récupérer le projet :

N.B. : Nous avons travaillé entièrement sur Windows, ainsi il est possible que cela dysfonctionne sur Linux ou Mac à cause d'un fonctionnement différent (ex: arborescence différente dans le dossier d'openFrameworks).

Option 1 : Clôner le dépôt GitHub

=> Vous pouvez clôner le projet qui est disponible sur le lien https://github.com/SamuelFreydier/MoteurBalistique

Veillez à le clôner dans le répertoire of_vX.X.X_vs_release\apps\myApps car il y a des chemins relatifs entre la solution et des bibliothèques d'openFrameworks. Vous pouvez ensuite ouvrir la solution et cela devrait compiler.

Option 2 : Créer un projet openFrameworks

=> Vous pouvez créer un projet de toutes pièces en utilisant of_vX.X.X_vs_release\projectGenerator\projectGenerator.exe

Lors de la création du projet, il est important d'ajouter l'addon ofxGui permettant d'avoir une interface de configuration des paramètres.

Ensuite, vous pouvez ouvrir la solution Visual ainsi générée et ajouter les fichiers source (et les headers) au projet dans le dossier src.