# Bifrost

Bifrost is a game library I am working on. 

Currently it is still in infant stages. Some of its features:

* It tries to abstract some of the lower level stuff in OpenGL such as vertex/index buffers, shaders, textures etc. 
* Resource (texture/model etc) management
* Keyboard / mouse input abstraction
* Vector & Matrix library.
* Worker threads with thread pooling
* Easier camera controls
* Warcraft3 model loading
* Debug visualizations (Adding debug lines etc)

I wanted to make a more object oriented approach in this library. For example implementing gTickable abstrack class will tick your class on each game tick. gUpdatable will call update() on your class on each fixed update (naming is hard). Or gRenderable functions will be rendered when necessary.

I don't want to abstract OpenGL completetly from application side. The user still will be able to call OpenGL calls if he wants. It is probably more dangerous but in my opinion gives programmer a lot more opportunity. It is also easier :)

## Usage

Bifrost comes with an example game (that I probabl commit more time) which is called Valhalla. It is a random terrain generator. It is probably not the best choice to show capabilities of library because most models are randomly generated, so most of the resource functions are unused. I should probably include another simple game in future. It does utilized worker threads though, meshes such trees or detailed version of terrain are generated randomly in background as player moves.

## Screenshots

Some example pictures from Valhalla.

![ss0](https://raw.githubusercontent.com/shultays/bifrost/master/ss0.png)
![ss1](https://raw.githubusercontent.com/shultays/bifrost/master/ss1.png)
![ss2](https://raw.githubusercontent.com/shultays/bifrost/master/ss2.png)

## Install

To compile, you will need OpenGL, glfw, glew dependincies. VS project files are included in project.

