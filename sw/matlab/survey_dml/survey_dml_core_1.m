%-------------------------------------------------------------------------------
  %
  %  Filename       : survey_dml_core_1.m
  %  Author         : Huang Leilei
  %  Created        : 2020-06-04
  %  Description    : core calculation of dml
  %
%-------------------------------------------------------------------------------

function idxAngRnd = survey_dml_core_1(NUMB_RND, NUMB_SMP, INDX_FIG, DATA_DIS_ANT, DATA_COE_WIN, DATA_ANG_OBJ, DATA_POW_OBJ, DATA_SNR, DATA_DLT_ANG_TST, DATA_RNG_ANG_TST)

%%*** DERIVED PARAMETER *********************************************************
DATA_ANG_TST     = -DATA_RNG_ANG_TST:DATA_DLT_ANG_TST:DATA_RNG_ANG_TST;
NUMB_ANT         = numel(DATA_DIS_ANT);
NUMB_OBJ         = numel(DATA_ANG_OBJ);
NUMB_ANG_TST     = numel(DATA_ANG_TST);
DATA_ANG_AMB_MAX = asin(1 / (DATA_DIS_ANT(2  ) - DATA_DIS_ANT(1)) / 2) / pi * 180;
fprintf('max non-ambiguity angle of objects is about %.2f degree\n', DATA_ANG_AMB_MAX);




%*** MAIN BODY *****************************************************************
%% prepare datP
datPTst = complex(zeros(NUMB_ANT, NUMB_ANT, NUMB_ANG_TST, NUMB_ANG_TST));
for idxAng0 = 1:NUMB_ANG_TST
    datAng0 = DATA_ANG_TST(idxAng0);
    for idxAng1 = (idxAng0+1):NUMB_ANG_TST
        datAng1 = DATA_ANG_TST(idxAng1);
        datA = [exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng0 / 180 * pi))     ...
               ,exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng1 / 180 * pi))]    ...
        ;
        datP = datA * (datA' * datA)^-1 * datA';
        datPTst(:, :, idxAng0, idxAng1) = datP;
    end
end


%% traverse
idxAngRnd = zeros(2, NUMB_RND);
for idxRnd = 1:NUMB_RND
    %% prepare datSig
    datSig = complex(zeros(NUMB_ANT, 1));
    for idxObj = 1:NUMB_OBJ
        datAmp = 10^(DATA_POW_OBJ(idxObj) / 20) / 2^0.5;
        datPha = exp(-1i * 2 * pi * rand);
        datSig = datSig + datAmp .* exp(1i * 2 * pi * DATA_DIS_ANT * sin(DATA_ANG_OBJ(idxObj) / 180 * pi)) * datPha;
    end


    %% prepare datR
    datRSum = complex(zeros(NUMB_ANT));
    for idxSmp = 1:NUMB_SMP
        datRTmp = awgn(datSig, DATA_SNR, 'measured');
        datRTmp = datRTmp .* DATA_COE_WIN;
        datRSum = datRSum + datRTmp * datRTmp';
    end
    datR = datRSum / NUMB_SMP;
    %% prepare datR
    datSmpSum = complex(zeros(NUMB_ANT, 1));
    for idxSmp = 1:NUMB_SMP
        datSmpTmp = awgn(datSig, DATA_SNR, 'measured');
        datSmpTmp = datSmpTmp .* DATA_COE_WIN;
        datSmpSum = datSmpSum + datSmpTmp;
    end
    datX = datSmpSum / NUMB_SMP;
    datR = datX * datX';


    %% get best cost and index
    datPowTst = zeros(NUMB_ANG_TST, NUMB_ANG_TST) - inf;
    datPowBst = -inf;
    for idxAng0 = 1:NUMB_ANG_TST
        for idxAng1 = idxAng0+1:NUMB_ANG_TST
            datPowTmp = datPTst(:, :, idxAng0, idxAng1) * datR;
            datPow = 20 * log10(abs(trace(datPowTmp)));
            %datPowTmpA = datPTst(:, :, idxAng0, idxAng1) * datRTmp;
            %datPowA = 20 * log10(norm(datPowTmpA));
            if datPow > datPowBst
                datPowBst = datPow;
                idxAngRnd(:, idxRnd) = [idxAng0, idxAng1];
            end
            datPowTst(idxAng0, idxAng1) = datPow;
        end
    end


    %% plot
    if NUMB_RND == 1
        % open figure
        figure(INDX_FIG); INDX_FIG = INDX_FIG + 1;
        set(gcf, 'position', [800, 50, 1000, 850]);

        % surface
        subplot(2,2,1);
        surface(DATA_ANG_TST, DATA_ANG_TST, datPowTst - max(max(datPowTst)), 'LineStyle', 'none');
        hold on;
        % peak
        datAng0 = DATA_ANG_TST(idxAngRnd(1, idxRnd));
        datAng1 = DATA_ANG_TST(idxAngRnd(2, idxRnd));
        plot(DATA_ANG_TST, zeros(1, NUMB_ANG_TST) + datAng0, '--');
        plot(zeros(1, NUMB_ANG_TST) + datAng1, DATA_ANG_TST, '--');
        text(datAng1 + 3, datAng0 + 5, [num2str(datAng0,'%.1f'), ', ', num2str(datAng1,'%.1f')]);
        hold off;
        % tune subplot
        grid on;
        title('dml surface');
        xlabel('angle (degree)');
        ylabel('angle (degree)');

        % surface
        subplot(2,2,2);
        idxAng0 = idxAngRnd(1, idxRnd);
        idxAng1 = idxAngRnd(2, idxRnd);
        idxLocalR = 20;
        idxLocalX = max(1,idxAng0-idxLocalR):min(NUMB_ANG_TST,idxAng0+idxLocalR);
        idxLocalY = max(1,idxAng1-idxLocalR):min(NUMB_ANG_TST,idxAng1+idxLocalR);
        surface(DATA_ANG_TST(idxLocalY), DATA_ANG_TST(idxLocalX), datPowTst(idxLocalX, idxLocalY) - max(max(datPowTst)));
        hold on;
        % peak
        datAng0 = DATA_ANG_TST(idxAngRnd(1, idxRnd));
        datAng1 = DATA_ANG_TST(idxAngRnd(2, idxRnd));
        plot(DATA_ANG_TST(idxLocalY), zeros(1, numel(idxLocalY)) + datAng0, '--');
        plot(zeros(1, numel(idxLocalX)) + datAng1, DATA_ANG_TST(idxLocalX), '--');
        text(datAng1 + 3, datAng0 + 5, [num2str(datAng0,'%.1f'), ', ', num2str(datAng1,'%.1f')]);
        hold off;
        % tune subplot
        view([-1, -1, 0]);
        grid on;
        title('dml surface');
        xlabel('angle (degree)');
        ylabel('angle (degree)');

        % curve
        subplot(2,2,3);
        idxAng0 = idxAngRnd(1, idxRnd);
        idxAng1 = idxAngRnd(2, idxRnd);
        plot(DATA_ANG_TST, datPowTst(idxAng0, :));
        hold on;
        % peak
        datAng = DATA_ANG_TST(idxAng1);
        datPow = datPowTst(idxAng0, idxAng1);
        plot(datAng, datPow, 'xk', 'MarkerSize', 10);
        text(datAng - 0.5, datPow + 1.5,                                            ...
            ['\downarrow', num2str(datAng,'%.2f'), ', ', num2str(datPow,'%.2f')]    ...
        );
        hold off;
        % tune subplot
        grid on;
        axis([min(DATA_ANG_TST), max(DATA_ANG_TST), max(max(datPowTst)) - 10, max(max(datPowTst)) + 5])
        title('dml curve');
        xlabel('angle (degree)');
        ylabel('power (dB)');

        % curve
        subplot(2,2,4);
        idxAng0 = idxAngRnd(1, idxRnd);
        idxAng1 = idxAngRnd(2, idxRnd);
        plot(DATA_ANG_TST, datPowTst(:, idxAng1));
        hold on;
        % peak
        datAng = DATA_ANG_TST(idxAng0);
        datPow = datPowTst(idxAng0, idxAng1);
        plot(datAng, datPow, 'xk', 'MarkerSize', 10);
        text(datAng - 0.5, datPow + 1.5,                                            ...
            ['\downarrow', num2str(datAng,'%.2f'), ', ', num2str(datPow,'%.2f')]    ...
        );
        hold off;
        % tune subplot
        grid on;
        axis([min(DATA_ANG_TST), max(DATA_ANG_TST), max(max(datPowTst)) - 10, max(max(datPowTst)) + 5])
        title('dml curve');
        xlabel('angle (degree)');
        ylabel('power (dB)');

        % save figure
        fig = getframe(gcf);
        img = frame2im(fig);
        imwrite(img, 'dump/surface.png');
    end
end
