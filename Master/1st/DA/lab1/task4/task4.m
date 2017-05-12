% init A(3;10)
Ax = [-3 0 3];
Ay = [-10 0 10];

% center - (0,0), radius - 4
makeCircle(0, 0, 4);

% draw the vertical
plot(Ax, Ay, 'r'); 
text(3.2, 10.2, 'A');

x1 = 0; x2 = 3; x3 = 3;
y1 = 0; y2 = 0; y3 = 10;

% find the distance between A and O
d1 = sqrt(((x1 - x2)^2) + ((y1 - y2)^2));
d2 = sqrt(((x2 - x3)^2) + ((y2 - y3)^2));
d3 = sqrt((d1^2) + (d2^2));
d4 = 4;

% find the intersection point H
px = d4 * d1 / d3
py = d4 * d2 / d3

plot(px, py, 'ok'); 
text(px+0.5, py+0.5, 'H');
legend('O - окружность, R=4', 'AH - перпендикул€р', 'Location', 'northwest', 'Orientation', 'vertical');
xlabel('X');
ylabel('Y');
grid on;
