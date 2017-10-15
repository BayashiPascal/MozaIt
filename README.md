# MozaIt
MozaIt is a C library to generate a dot representation of a picture. It is also a web application to use this library through a webpage.

The representation is made by packing circles of fixed sizes, starting with the biggest and going down the smallest, at location where the difference between average color inside the circles and the color at the center of the circle is below a threshold. The threshold and the range of sizes are defined by the user. The sizes are constraint to powers of the golden ratio. Finally a margin parameter allows the user to control the minimum spacing between circles. As a background, the algorithm allows the user to choose a color, or to let the algorithm fill in with the average of the colors on the whole source image.

The web application allows the user to use the software in a straightforward way. After downloading a source image and selecting the value of parameters, the user gets the result image in one click. The web application limits the size of source images to less than 500x500 pixels to avoid very long computation time and load on the server. Images are deleted when the user leaves the page to protect his privacy.

http://www.bayashiinjapan.net/MozaIt/

MozaIt has two external dependancies: JQuery which can be found here: https://jquery.com/download/ and animate.css, wich can be found here: https://daneden.github.io/animate.css/
