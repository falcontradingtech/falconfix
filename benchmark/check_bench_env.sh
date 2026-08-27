#!/bin/bash

# Verifica se o script está sendo executado como root
if [ "$EUID" -ne 0 ]; then
    echo "Este script precisa ser executado como root (use sudo)."
    exit 1
fi

echo "=== Configurando CPU para o modo Performance ==="

echo "=== Libera perf para acessos ao kernel ==="
sysctl -w kernel.perf_event_paranoid=1

# Define o governor como 'performance' para todas as CPUs
for cpu in /sys/devices/system/cpu/cpu[0-9]*/cpufreq/scaling_governor; do
    if [ -f "$cpu" ]; then
        echo "performance" > "$cpu" 2>/dev/null
        if [ $? -eq 0 ]; then
            echo "$(basename $(dirname $cpu)): Configurado para performance"
        else
            echo "$(basename $(dirname $cpu)): Falha ao configurar (verifique permissões ou suporte)"
        fi
    else
        echo "Nenhum governor encontrado para $(basename $(dirname $cpu))"
    fi
done

# (Opcional) Configura o estado do turbo (se suportado)
if [ -f /sys/devices/system/cpu/intel_pstate/status ]; then
    echo "active" > /sys/devices/system/cpu/intel_pstate/status 2>/dev/null
    if [ $? -eq 0 ]; then
        echo "Turbo Boost: Ativado"
    else
        echo "Turbo Boost: Falha ao ativar"
    fi
else
    echo "Turbo Boost: Não suportado ou não disponível"
fi

# Exibe informações sobre o governor atual
echo -e "\n=== CPU Governor Check ==="
for cpu in /sys/devices/system/cpu/cpu[0-9]*; do
    gov=$(cat $cpu/cpufreq/scaling_governor 2>/dev/null || echo "N/A")
    echo "$(basename $cpu): $gov"
done

# Exibe informações de frequência com cpupower
echo -e "\n=== Frequency Info (cpupower) ==="
if command -v cpupower &> /dev/null; then
    cpupower frequency-info | egrep "current policy|current CPU frequency|hardware limits"
else
    echo "Instale o pacote cpupower: sudo apt install linux-tools-common"
fi

# Verifica temperaturas
echo -e "\n=== Thermal Throttling Check ==="
if command -v sensors &> /dev/null; then
    sensors | egrep "Core|Package"
else
    echo "Instale o pacote lm-sensors para ver temperaturas: sudo apt install lm-sensors"
fi

# Exibe estado do turbo (se suportado)
echo -e "\n=== Max Turbo State (if supported) ==="
cat /sys/devices/system/cpu/intel_pstate/status 2>/dev/null || echo "N/A"
