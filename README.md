# Longest Common Subsequence (LCS) - Sequential, Pthread, and CUDA Implementations

Cet algorithme permet de trouver la **Plus Longue Sous-séquence Commune (LCS)** entre deux chaînes de caractères. La LCS est la séquence de caractères la plus longue qui apparaît dans le même ordre dans les deux chaînes sans qu'ils soient nécessairement consécutifs. 

Par exemple, pour les chaînes **"ACDBE"** et **"ABCDE"**, la plus longue sous-séquence commune est **"ACDE"**.

Ce projet implémente l'algorithme **LCS** en utilisant trois approches différentes :
1. **Séquentielle (monothread)** - `FinalS.c`
2. **Parallèle avec Pthreads** - `Final.c`
3. **Parallèle sur GPU avec CUDA** - `prog.cu`

Le code est testé à l'aide de séquences d'ADN générées aléatoirement et enregistrées dans les fichiers texte `X_<size>.txt` et `Y_<size>.txt`.

## Structure du projet
```
📂 Répertoire du projet
├── FinalS.c               # Implémentation séquentielle de LCS
├── Final.c                # Implémentation parallèle de LCS avec Pthreads
├── prog.cu                # Implémentation parallèle de LCS avec CUDA
├── tester.py              # Script pour générer les fichiers d'entrée (séquences ADN)
├── run_tests.sh           # Script pour exécuter toutes les implémentations et sauvegarder les résultats
├── output.log             # Fichier de journal contenant les résultats d'exécution
├── X_<size>.txt           # Fichier d'entrée pour la chaîne X (tailles variées)
├── Y_<size>.txt           # Fichier d'entrée pour la chaîne Y (tailles variées)
└── README.md              # Documentation du projet
```

## Instructions d'installation

### Prérequis
Assurez-vous que les outils et bibliothèques suivants sont installés :
- **GCC** pour compiler les programmes en C
- **Bibliothèque Pthreads** (généralement préinstallée sur Linux)
- **NVIDIA CUDA Toolkit** pour l'exécution sur GPU
- **Python 3** pour la génération des tests

### Dépendances
Pour CUDA :
- Le compilateur NVIDIA CUDA (`nvcc`) doit être installé et accessible.

### Description des fichiers
1. **`tester.py`** : Génère des séquences ADN aléatoires de différentes longueurs et les enregistre dans des fichiers texte.
   - Exécutez ce script **une seule fois** avant les tests.
   - Sortie : fichiers `X_<size>.txt` et `Y_<size>.txt`.

   **Utilisation :**
   ```bash
   python3 tester.py
   ```

2. **`run_tests.sh`** : Exécute les implémentations séquentielle, Pthread et CUDA avec les fichiers d'entrée générés et enregistre les résultats dans `output.log`.
   
   **Utilisation :**
   ```bash
   chmod +x run_tests.sh  # Rendre le script exécutable
   ./run_tests.sh         # Exécuter le script
   ```

3. **Fichiers de code** :
   - `FinalS.c` : Implémentation séquentielle.
   - `Final.c` : Implémentation multithread avec Pthreads.
   - `prog.cu` : Implémentation CUDA pour l'exécution sur GPU.

## Compilation et exécution

### 1. Séquentielle (FinalS.c)
Compilation :
```bash
gcc -o FinalS FinalS.c
```
Exécution :
```bash
./FinalS X_<size>.txt Y_<size>.txt
```

### 2. Pthread (Final.c)
Compilation :
```bash
gcc -o Final -lpthread Final.c
```
Exécution :
```bash
./Final X_<size>.txt Y_<size>.txt
```

### 3. CUDA (prog.cu)
Compilation :
```bash
nvcc -o prog prog.cu
```
Exécution :
```bash
./prog X_<size>.txt Y_<size>.txt
```

## Workflow de test
1. **Générer les fichiers d'entrée** :
   Utilisez `tester.py` pour générer les fichiers `X_<size>.txt` et `Y_<size>.txt` avec des séquences ADN aléatoires.
   ```bash
   python3 tester.py
   ```

2. **Exécuter les implémentations** :
   Exécutez toutes les versions (Séquentielle, Pthread, CUDA) en utilisant le script `run_tests.sh`.
   ```bash
   ./run_tests.sh
   ```

3. **Vérifier les résultats** :
   Les résultats sont enregistrés dans `output.log`. Vérifiez la précision et comparez les temps d'exécution.

## Exemple `run_tests.sh`
Voici un aperçu du script `run_tests.sh` :
```bash
#!/bin/bash

# Compiler toutes les implémentations
gcc -o FinalS FinalS.c
gcc -o Final -lpthread Final.c
nvcc -o prog prog.cu

# Exécuter les tests et sauvegarder les résultats
output_file="output.log"
echo "Exécution des tests..." > $output_file

for size in 5 25 150 500 1000 2500 5000 10000 15000 25000 30000 40000; do
    echo "\nTaille testée : $size" >> $output_file

    echo "Exécution séquentielle :" >> $output_file
    ./FinalS X_${size}.txt Y_${size}.txt >> $output_file

    echo "Exécution Pthread :" >> $output_file
    ./Final X_${size}.txt Y_${size}.txt >> $output_file

    echo "Exécution CUDA :" >> $output_file
    ./prog X_${size}.txt Y_${size}.txt >> $output_file

done

echo "Tests terminés. Résultats sauvegardés dans $output_file."
```

## Résultats
Les temps d'exécution pour chaque implémentation sont comparés pour différentes tailles d'entrée dans le fichier `output.log`. Cela permet de démontrer les gains de performance obtenus grâce à la parallélisation de l'algorithme LCS.

## Notes
- Modifiez `tester.py` si vous avez besoin de tailles d'entrée différentes.
- Assurez-vous que tous les outils (GCC, nvcc, Python) sont correctement configurés sur votre système.

## Licence
Ce projet est à des fins éducatives.

---
**Auteur** : Benameur Tarek , Touil Nihel  
**Cours** : High Performance Computing (HPC)            
**Enseignant** : Haichour Amina Selma
**Date** : 12/16/2024

