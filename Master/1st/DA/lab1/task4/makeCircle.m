% creates the circle
function makeCircle(x0, y0, r)
    % phi is the angle
    phi = 0:0.01:2*pi;
    
    % parametric equation
    xp = r * cos(phi);
    yp = r * sin(phi);
    
    % draw graph
    plot(x0 + xp, y0 + yp, 'b');
    axis([-9 9, -6 12]);
    axis('square');
    text(x0+0.2, y0+0.2, 'O');
    grid on;
    hold on;
end