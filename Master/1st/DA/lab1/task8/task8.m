% read an image and put it into Img[] array
Img = imread('emily.jpg'); 

% convert an rgb image into the gray scale image
Img = rgb2gray(Img); 

% create 3 copies of the current image (for each channel)
Img = repmat(double(Img)./255, [1 1 3]);

color = input('Input color (r/g/b): ', 's'); 

% zero those channels which we don't need to show
if (strcmp(color, 'r'))  
    Img(:,:,2) = 0;  
    Img(:,:,3) = 0;  
elseif (strcmp(color, 'g')) 
    Img(:,:,1) = 0;  
    Img(:,:,3) = 0;  
elseif (strcmp(color, 'b'))  
    Img(:,:,1) = 0; 
    Img(:,:,2) = 0;  
end

% display an image
imshow(Img);
