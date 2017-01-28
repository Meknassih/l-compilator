f(entier $a, entier $b)  # declaration d'une fonction a deux arguments
entier $c, entier $k;    # declaration de deux variables locales
{                        # debut d?un bloc d'instruction
$k = $a + $b;          # affectation et expression arithmetique
retour $k;             # valeur de retour de la fonction
}                        # fin du bloc d'instruction
main()                   # point d'entree dans le programme
entier $d;
{
$d = f($d, 2);         # affectation et appel de fonction
ecrire($d + 1);        # appel de la fonction prédéfinie ecrire
}