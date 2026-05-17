# liste le contenu de 'code', puis modifie le résultat pour avoir juste le nom de fichier sans le .c
for f in `ls code/*.c | sed -r s/"code\/(.*)\.c"/"\1"/`; do
  # pour chaque fichier, compile et met la sortie dans le répertoire courant
  gcc -o $f code/$f.c
done
