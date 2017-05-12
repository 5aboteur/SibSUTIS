% creates & display the triangle
function makeTriangle(a1, a2, b1, b2, c1, c2, name)
    % init x and y
    x = [a1 b1 c1 a1];
    y = [a2 b2 c2 a2];
    
    if name == 'tr1'
        % create graph with stuff
        plot(x, y, 'b');
        axis([a1-1 c1+1 a2-1 c2+1]);
        axis('square');
        hold on;
        
        % draw the vertical
        plot([-1 2], [4 1], 'r', 2, 1, 'ok')
        text(2.2, 0.8, 'H');
        
        legend('ABC - треугольник', 'BH - перпендикул€р', 'Location', 'southeast', 'Orientation', 'vertical');
        text(a1-0.2, a2-0.2, 'A');
        text(b1-0.3, b2+0.2, 'B');
        text(c1+0.2, c2+0.2, 'C');
        xlabel('X');
        ylabel('Y');
        grid on;
    elseif name == 'tr2'
        % create graph with stuff
        plot(x, y, 'b'); 
        axis([b1-0.5 c1+0.5 a2-2 c2+1]);
        axis('square');
        hold on;
        
        % draw the vertical
        plot([-1 0.86], [4 1.25], 'r', 0.86, 1.25, 'ok')
        text(0.86, 1, 'H');
        
        % draw the imaginary line
        plot([a1 0.86], [a2 1.25], '--b');
        
        legend('A1BC - треугольник', 'BH - перпендикул€р', 'Location', 'southeast', 'Orientation', 'vertical');
        text(a1+0.2, a2-0.2, 'A1');
        text(b1-0.3, b2+0.2, 'B');
        text(c1+0.2, c2+0.2, 'C');
        xlabel('X');
        ylabel('Y');
        grid on;
    else
        disp('Nothing to display');
    end
end