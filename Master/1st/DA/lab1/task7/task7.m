% choose figure
id = input('Choose figure cube(q) or cone(c): ', 's');

% this goes for a cube
if strcmp(id, 'q')
    r = input('Input r: ');
    % init vertices
    vert = [-1 -1 -1; 
            -1  1 -1; 
             1  1 -1; 
             1 -1 -1; 
            -1 -1  1; 
            -1  1  1; 
             1  1  1; 
             1 -1  1];
         
    % init faces
    face = [1 2 3 4; 
            2 6 7 3; 
            4 3 7 8; 
            1 5 8 4; 
            1 2 6 5; 
            5 6 7 8];
        
    % patch them    
    patch('Faces', face, 'Vertices', vert * r, 'FaceAlpha', 0.1);
    grid on;
    axis('square');
    view(3);
% this goes for a cone
elseif strcmp(id, 'c')
    h = input('Input height: ');
    r = input('Input length: ');
    [X,Y,Z] = cylinder([r 0]);
    Z = Z*h;
    surf(X, Y, Z, 'FaceAlpha', 0.1)
    axis('square')
end
