% init x
x = -4:0.5:6;

% init y
y = (x + 2).*(x - 4);

% find an extreme value of the function
[xmin, ymin] = fminbnd('(x + 2) .* (x - 4)', -4, 6)

% create graph with stuff
plot(x, y, xmin, ymin, 'ok');
legend('y = (x + 2)(x - 4)', 'Location', 'northeast', 'Orientation', 'vertical');
xlabel('X');
ylabel('Y');
grid;