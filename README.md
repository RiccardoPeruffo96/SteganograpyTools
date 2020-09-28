# SteganograpyTools
Some security features for everyone

With this alpha version, 
my target is to give the possibility to everyone with this code to use the most common and powerful securities algorithms.
Now it's a little bit difficult to use, but with time I'll add some features like a GUI and a better implementation of config file, with a login form.

But if you want to use it now, you need to know this:
1) config.json it's a essential file that you can edit like a common txt file;
2) config.json needs to be in the same program's directory;
3) config.json tells at the program what are the files with you want to work;
4) without config.json, the program starts, but you can use only diffie-hellman algorithm, or will crash;

How edit config.json:
1) there are 5 macro sections: hide, seek, encrypt, decrypt and remove_file. DON'T EDIT THESE NAMES.
2) every macro section has a one or more subsections. At example, hide has file_input, img_input and img_output. DON'T EDIT THESE SUBSECTIONS' NAMES.
3) every subsection has one labeled field. Don't edit the label, but the field is the name of your file target.
4) so: 
"hide" :{ "file_input" :{ "name" = "file.zip" }, "img_input" :{ "name" = "imgInput.jpg" }, "img_output" :{ "name" = "imgOutput" }},
If you want to use 'hide' you can edit only 3 names: "file.zip", "imgInput.jpg", "imgOutput".
First represent the file to hide inside an image, using a simply steganography algorithm.
Second is input image's name.
Third is output image's name that will be created: it seems a copy of the input's image.
First and second need to be in the same program's directory.

"seek" :{ "img_input" :{ "name" = "imgOutput.png" }, "file_output" :{ "name" = "file_output.zip" }},
If you want to use 'seek' you can edit only 2 names: "imgOutput.png", "file_output.zip".
First represent the image after using 'hide'.
Second is the name you want to use for the new file that will be created.
First needs to be in the same program's directory.

"encrypt" :{ "file_input" :{ "name" = "file_i.txt" }, "file_output" :{ "name" = "file_o.txt" }},
If you want to use 'encrypt' you can edit only 2 names: "file_i.txt", "file_o.txt".
First represent the file that you want to encrypt.
Second is the name you want to use for the new file that will be created.
First needs to be in the same program's directory.

"decrypt" :{ "file_input" :{ "name" = "file_i.txt" }, "file_output" :{ "name" = "file_o.txt" }},
If you want to use 'decrypt' you can edit only 2 names: "file_i.txt", "file_o.txt".
First represent the file that you want to decrypt.
Second is the name you want to use for the new file that will be created.
First needs to be in the same program's directory.

"remove_file" :{ "file_input" :{ "name" = "file_remove.zip" }},
If you want to use 'remove_file' you can edit only 1 name: "file_remove.zip".
Represent the file that you want to cancel and destroy it.
It needs to be in the same program's directory.

"hide" is used with commands: 1, 5, 12.
"seek" is used with commands: 2, 6, 13.
"encrypt" is used with commands: 3, 10.
"decrypt" is used with commands: 4, 11.
"remove_file" is used with command 9.
