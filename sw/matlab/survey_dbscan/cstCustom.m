%-------------------------------------------------------------------------------
  %
  %  Filename       : cstCustom.m
  %  Author         : Huang Leilei
  %  Created        : 2020-07-19
  %  Description    : survey_dbscan
  %
%-------------------------------------------------------------------------------
  %
  %  Modified       : 2020-08-01 by HLL
  %  Description    : actual unit adopted
  %  Modified       : 2020-09-06 by HLL
  %  Description    : cost renewed
  %
%-------------------------------------------------------------------------------

function cost = cstCustom(single, multi)

IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;
UNIT = [0.3, 0.3, 0.2, 0.2, 1];
SCLR = [0.8, 0.8, 1  , 1  , 0];
%{
unit = [0.3, 0.2, 0.5 / 180 * pi, 1];
sclr = [1, 1, 1, 0];
[num, siz] = size(multi);
dlt = zeros(num, siz);
for i = 1 : siz
  dlt(:,i) = multi(:,i) - single(i);
  dlt(:,i) = dlt(:,i) ./ unit(i) .* sclr(i);
end
cost = dlt .^ 2;
cost = sum(cost, 2);
cost = cost .^ 0.5;
%}

temp = single;
single(1) = temp(IDX_RNG) * sin(temp(IDX_ANG));
single(2) = temp(IDX_RNG) * cos(temp(IDX_ANG));
single(3) = temp(IDX_VEL) * sin(temp(IDX_ANG));
single(4) = temp(IDX_VEL) * cos(temp(IDX_ANG));
single(5) = temp(IDX_SNR);
temp = multi;
multi(:,1) = temp(:,IDX_RNG) .* sin(temp(:,IDX_ANG));
multi(:,2) = temp(:,IDX_RNG) .* cos(temp(:,IDX_ANG));
multi(:,3) = temp(:,IDX_VEL) .* sin(temp(:,IDX_ANG));
multi(:,4) = temp(:,IDX_VEL) .* cos(temp(:,IDX_ANG));
multi(:,5) = temp(:,IDX_SNR);
[num, siz] = size(multi);
dlt = zeros(num, siz);
for i = 1 : siz
  dlt(:,i) = multi(:,i) - single(i);
  dlt(:,i) = dlt(:,i) ./ UNIT(i) .* SCLR(i);
end
cost = dlt .^ 2;
cost = sum(cost, 2);
cost = cost .^ 0.5;
