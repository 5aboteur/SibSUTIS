% id is the angle
id = input('Input an angle (90, 180, 270, 0): ');

% read the image
Img = imread('emily.jpg');

% rotate the image
if (id == 90) || (id == 180) || (id == 270)
    rImg = imcomplement(Img);
    imshow(imrotate(rImg, id))
elseif (id == 0)
    rImg = imcomplement(Img);
    imshow(rImg)
end
