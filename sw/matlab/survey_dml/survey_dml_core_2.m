%-------------------------------------------------------------------------------
  %
  %  Filename       : survey_dml_core_2.m
  %  Author         : Huang Leilei
  %  Created        : 2020-06-04
  %  Description    : core calculation of dml
  %
%-------------------------------------------------------------------------------

function idxAngRnd = survey_dml_core_2(NUMB_RND, NUMB_SMP, INDX_FIG, DATA_DIS_ANT, DATA_COE_WIN, DATA_ANG_OBJ, DATA_POW_OBJ, DATA_SNR, DATA_DLT_ANG_TST, DATA_RNG_ANG_TST)

%%*** DERIVED PARAMETER *********************************************************
DATA_ANG_TST = -DATA_RNG_ANG_TST:DATA_DLT_ANG_TST:DATA_RNG_ANG_TST;
NUMB_ANT     = numel(DATA_DIS_ANT);
NUMB_OBJ     = numel(DATA_ANG_OBJ);
NUMB_ANG_TST = numel(DATA_ANG_TST);




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
end
