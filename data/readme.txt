
An important note for users!

After the version of Geant4 10.0.6, STL vector (std::vector) is started 
to be used for the definition of material property as an alternative to 
C-style arrays. For now, if you change the number of data in a text file,
which is located under /data, you should also change the corresponding array 
size in NuSDMaterial.cc. This will be fixed as soon as possible. Namely, 
all C-style arrays will be converted to std::vector in the next version of NuSD.

