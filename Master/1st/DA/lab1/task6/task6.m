% id - choice
id = input('Choose figure - sphere(s) or pyramid(p): ', 's');

% goes for a sphere
if strcmp(id, 's')
    R = input('Input radius: ');
    [X,Y,Z] = sphere(30);
    X = X*R;
    Y = Y*R;
    Z = Z*R;
    surf(X, Y, Z, 'FaceAlpha', 0.1);
    axis('square');
% and this one for a pyramid
elseif strcmp(id, 'p')
    h = input('Input height: ');
    l = input('Input length: ');
    [X,Y,Z] = cylinder([l 0], 4);
    Z = Z*h;
    surf(X, Y, Z, 'FaceAlpha', 0.1);
    axis('square');
end
