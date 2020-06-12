%-------------------------------------------------------------------------------
  %
  %  Filename       : survey_dml_core_4.m
  %  Author         : Huang Leilei
  %  Created        : 2020-06-05
  %  Description    : core calculation of dml
  %
%-------------------------------------------------------------------------------

function idxAngRnd = survey_dml_core_4(NUMB_RND, NUMB_SMP, INDX_FIG, DATA_DIS_ANT, DATA_COE_WIN, DATA_ANG_OBJ, DATA_POW_OBJ, DATA_SNR, DATA_DLT_ANG_TST, DATA_RNG_ANG_TST)

%%*** DERIVED PARAMETER *********************************************************
DATA_ANG_TST = -DATA_RNG_ANG_TST:DATA_DLT_ANG_TST:DATA_RNG_ANG_TST;
NUMB_ANT     = numel(DATA_DIS_ANT);
NUMB_OBJ     = numel(DATA_ANG_OBJ);
NUMB_ANG_TST = numel(DATA_ANG_TST);
DATA_STP_RGH = floor(sqrt(NUMB_ANG_TST));
DATA_RNG_RFN = ceil(DATA_STP_RGH);




%*** MAIN BODY *****************************************************************
%% prepare datV and datC
datVTst = complex(zeros(NUMB_ANT, NUMB_ANG_TST));
for idxAng = 1:NUMB_ANG_TST
    datAng = DATA_ANG_TST(idxAng);
    datVTst(:, idxAng) = exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng / 180 * pi));
end
datCTst = complex(zeros(2, 2, NUMB_ANG_TST, NUMB_ANG_TST));
for idxAng0 = 1:NUMB_ANG_TST
    datAng0 = DATA_ANG_TST(idxAng0);
    for idxAng1 = (idxAng0+1):NUMB_ANG_TST
        datAng1 = DATA_ANG_TST(idxAng1);
        datA = [exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng0 / 180 * pi))     ...
               ,exp(1i * 2 * pi * DATA_DIS_ANT * sin(datAng1 / 180 * pi))]    ...
        ;
        datC = (datA' * datA)^-1;
        datCTst(:, :, idxAng0, idxAng1) = datC;
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


    %% prepare datVX
    datVXTst = complex(zeros(NUMB_ANG_TST, 1));
    for idxAng = 1:NUMB_ANG_TST
        datVXTst(idxAng) = datVTst(:, idxAng)' * datX;
    end


    %% get best cost and index (rough)
    datPowTst = zeros(NUMB_ANG_TST, NUMB_ANG_TST) - inf;
    datPowBst = -inf;
    for idxAng0 = 1:DATA_STP_RGH:NUMB_ANG_TST
        for idxAng1 = 2:DATA_STP_RGH:NUMB_ANG_TST
            if idxAng0 < idxAng1 && idxAng1 <= NUMB_ANG_TST
                datPowTmp = [datVXTst(idxAng0); datVXTst(idxAng1)]' * datCTst(:, :, idxAng0, idxAng1) * [datVXTst(idxAng0); datVXTst(idxAng1)];
                datPow = 10 * log10(abs(datPowTmp));
                if datPow > datPowBst
                    datPowBst = datPow;
                    idxAngRnd(:, idxRnd) = [idxAng0, idxAng1];
                end
                datPowTst(idxAng0, idxAng1) = datPow;
            end
        end
    end


    %% get best cost and index (refined)
    idxAngBstRgh0 = idxAngRnd(1, idxRnd);
    idxAngBstRgh1 = idxAngRnd(2, idxRnd);
    for idxAng0 = max(1,idxAngBstRgh0-DATA_RNG_RFN):min(NUMB_ANG_TST,idxAngBstRgh0+DATA_RNG_RFN)
        for idxAng1 = max(idxAng0+1,idxAngBstRgh1-DATA_RNG_RFN):min(NUMB_ANG_TST,idxAngBstRgh1+DATA_RNG_RFN)
            datPowTmp = [datVXTst(idxAng0); datVXTst(idxAng1)]' * datCTst(:, :, idxAng0, idxAng1) * [datVXTst(idxAng0); datVXTst(idxAng1)];
            datPow = 10 * log10(abs(datPowTmp));
            if datPow > datPowBst
                datPowBst = datPow;
                idxAngRnd(:, idxRnd) = [idxAng0, idxAng1];
            end
            datPowTst(idxAng0, idxAng1) = datPow;
        end
    end
end
